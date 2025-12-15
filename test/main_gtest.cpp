#include "factory.hpp"
#include "helpers.hpp"
#include "nodes.hpp"
#include "package.hpp"
#include "reports.hpp"
#include "simulation.hpp"
#include "storage_types.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace NetSim;

// ============================================================================
//                               PACKAGE TESTS
// ============================================================================

TEST(PackageTest, IDGenerationIsUnique) {
    Package p1;
    Package p2;
    EXPECT_NE(p1.get_id(), p2.get_id());
}

// ============================================================================
//                               QUEUE TESTS
// ============================================================================

TEST(PackageQueueTest, FIFO_Order) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p1 = q.pop();
    EXPECT_EQ(p1.get_id(), 1); // FIFO: First in, first out

    Package p2 = q.pop();
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageQueueTest, LIFO_Order) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p1 = q.pop();
    EXPECT_EQ(p1.get_id(), 2); // LIFO: Last in, first out (stack)

    Package p2 = q.pop();
    EXPECT_EQ(p2.get_id(), 1);
}

// ============================================================================
//                            BUSINESS LOGIC (NODES)
// ============================================================================

// Helper class for testing PackageSender (access to protected members)
class TestSender : public PackageSender {
  public:
    // Wrapper exposing the push_package method
    void push_package_public(Package &&p) { push_package(std::move(p)); }
    // Check if buffer is empty
    bool is_buffer_empty() const { return !buffer_.has_value(); }
};

TEST(PackageSenderTest, BufferClearedAfterSend) {
    TestSender sender;

    // Create a "Mock" receiver so the sender has somewhere to send to
    Storehouse receiver(1);
    sender.get_receiver_preferences().add_receiver(&receiver);

    // Insert a package
    sender.push_package_public(Package(100));
    EXPECT_FALSE(sender.is_buffer_empty());

    // Send
    sender.send_package();

    // Check: buffer should be empty, and package in storehouse
    EXPECT_TRUE(sender.is_buffer_empty());
    EXPECT_EQ(receiver.begin()->get_id(), 100);
}

TEST(ReceiverPreferencesTest, ProbabilityScaling) {
    ReceiverPreferences prefs;
    Storehouse s1(1), s2(2);

    prefs.add_receiver(&s1);
    // 1 receiver = 1.0 probability
    EXPECT_DOUBLE_EQ(prefs.get_preferences().at(&s1), 1.0);

    prefs.add_receiver(&s2);
    // 2 receivers = 0.5 probability each
    EXPECT_DOUBLE_EQ(prefs.get_preferences().at(&s1), 0.5);
    EXPECT_DOUBLE_EQ(prefs.get_preferences().at(&s2), 0.5);

    prefs.remove_receiver(&s1);
    // Back to 1.0 probability
    EXPECT_DOUBLE_EQ(prefs.get_preferences().at(&s2), 1.0);
}

TEST(ReceiverPreferencesTest, MockedGeneratorSelection) {
    // Define a generator that always returns 0.3
    auto fixed_gen = []() { return 0.3; };
    ReceiverPreferences prefs(fixed_gen);

    Storehouse s1(1), s2(2);
    prefs.add_receiver(&s1); // range [0, 0.5]
    prefs.add_receiver(&s2); // range (0.5, 1.0]

    // For value 0.3, we should hit the first range (s1)
    IPackageReceiver *selected = prefs.choose_receiver();
    EXPECT_EQ(selected, &s1);
}

TEST(RampTest, DeliveryInCorrectRound) {
    // Ramp delivers every 2 rounds (interval 2)
    Ramp ramp(1, 2);
    Storehouse receiver(1);
    ramp.get_receiver_preferences().add_receiver(&receiver);

    // Round 1: (1-1)%2 == 0 -> SHOULD be a delivery
    ramp.deliver_goods(1);
    ramp.send_package(); // Ramp immediately sends from buffer
    EXPECT_FALSE(receiver.begin() == receiver.end()); // Storehouse not empty

    // Round 2: (2-1)%2 == 1 -> NO delivery
    // Clear storehouse to check if anything arrives
    Storehouse receiver2(2);
    ramp.get_receiver_preferences().remove_receiver(&receiver);
    ramp.get_receiver_preferences().add_receiver(&receiver2);

    ramp.deliver_goods(2);
    ramp.send_package();
    EXPECT_TRUE(receiver2.begin() == receiver2.end()); // Storehouse empty
}

TEST(WorkerTest, ProcessingDurationAndForwarding) {
    // Worker processes for 2 rounds
    Worker worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Storehouse store(1);
    worker.get_receiver_preferences().add_receiver(&store);

    // Give it a package
    worker.receive_package(Package(50));

    // Round 1: Takes package in hand (start of work)
    worker.do_work(1);
    // Nothing should be sent, because work takes 2 rounds
    worker.send_package();
    EXPECT_TRUE(store.begin() == store.end()); // Storehouse empty

    // Round 2: Finishes work (1 + 2 - 1 = 2)
    worker.do_work(2);
    // Now the package should be in the worker's outgoing buffer. Sending.
    worker.send_package();

    // Check if it arrived
    ASSERT_FALSE(store.begin() == store.end());
    EXPECT_EQ(store.begin()->get_id(), 50);
}

TEST(StorehouseTest, ReceivingPackage) {
    Storehouse store(1);
    store.receive_package(Package(99));

    auto it = store.begin();
    ASSERT_NE(it, store.end()); // Cannot be empty
    EXPECT_EQ(it->get_id(), 99);
}

// ============================================================================
//                              FACTORY TESTS
// ============================================================================

TEST(FactoryTest, RemoveReceiverCleansUpConnections) {
    Factory factory;
    // Structure: Ramp(1) -> Worker(2) -> Storehouse(3)
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(
        Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(3));

    auto &ramp = *factory.find_ramp_by_id(1);
    auto &worker = *factory.find_worker_by_id(2);
    auto &store = *factory.find_storehouse_by_id(3);

    // Create connections
    ramp.get_receiver_preferences().add_receiver(&worker);
    worker.get_receiver_preferences().add_receiver(&store);

    // TEST 1: Removing Worker should remove it from Ramp's preferences
    factory.remove_worker(2);

    // Check if Ramp is now orphaned
    EXPECT_TRUE(ramp.get_receiver_preferences().get_preferences().empty());

    // Recreate worker for the second test
    factory.add_worker(
        Worker(4, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    auto &new_worker = *factory.find_worker_by_id(4);
    new_worker.get_receiver_preferences().add_receiver(&store);

    // TEST 2: Removing Storehouse should remove it from Worker's preferences
    factory.remove_storehouse(3);

    EXPECT_TRUE(
        new_worker.get_receiver_preferences().get_preferences().empty());
}

TEST(FactoryTest, NetworkConsistency) {
    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_storehouse(Storehouse(2));
    factory.add_worker(Worker(3, 1,
    std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    // 1. Inconsistent (Ramp has no output)
    EXPECT_FALSE(factory.is_consistent());

    auto& ramp = *factory.find_ramp_by_id(1);
    auto& store = *factory.find_storehouse_by_id(2);
    auto& worker = *factory.find_worker_by_id(3);

    // 2. Inconsistent (Ramp -> Worker -> nowhere)
    ramp.get_receiver_preferences().add_receiver(&worker);
    EXPECT_FALSE(factory.is_consistent());

    // 3. Consistent (Ramp -> Worker -> Storehouse)
    worker.get_receiver_preferences().add_receiver(&store);
    EXPECT_TRUE(factory.is_consistent());

    // 4. Consistent (Ramp -> Storehouse)
    ramp.get_receiver_preferences().remove_receiver(&worker);
    ramp.get_receiver_preferences().add_receiver(&store);
    EXPECT_TRUE(factory.is_consistent());
}


// ============================================================================
//                             SIMULATION TESTS
// ============================================================================

TEST(SimulationTest, IntervalReportingLogic) {
    // Interval 2: Reports at 1, 3, 5...
    IntervalReportNotifier notifier(2);
  
    EXPECT_TRUE(notifier.should_generate_report(1));
    EXPECT_FALSE(notifier.should_generate_report(2));
    EXPECT_TRUE(notifier.should_generate_report(3));
}

TEST(SimulationTest, SpecificTurnsReportingLogic) {
    // Reports at 2 and 5
    SpecificRoundsReportNotifier notifier(std::set<Time>{2, 5});
  
    EXPECT_FALSE(notifier.should_generate_report(1));
    EXPECT_TRUE(notifier.should_generate_report(2));
    EXPECT_FALSE(notifier.should_generate_report(3));
    EXPECT_TRUE(notifier.should_generate_report(5));
}

TEST(SimulationTest, SimulationLoopExecution) {
    Factory f;
    f.add_ramp(Ramp(1, 1));
    f.add_storehouse(Storehouse(2));
    f.find_ramp_by_id(1)->get_receiver_preferences().add_receiver(&(*f.find_storehouse_by_id(2)));

    int report_count = 0;

    // Reporting function (lambda) that counts calls
    auto report_func = [&report_count](Factory&, Time) {
        report_count++;
    };

    // Run for 5 turns
    simulate(f, 5, report_func); // passing report_func that's just called at the end of each round

    EXPECT_EQ(report_count, 5);
}

TEST(SimulationTest, ComplexSimulationCheck) {
    // Ramp (interval 1) -> Worker (proc_time 1) -> Storehouse
    Factory f;
    f.add_ramp(Ramp(1, 1));
    f.add_worker(Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    f.add_storehouse(Storehouse(3));

    auto r = &(*f.find_ramp_by_id(1));
    auto w = &(*f.find_worker_by_id(2));
    auto s = &(*f.find_storehouse_by_id(3));

    r->get_receiver_preferences().add_receiver(w);
    w->get_receiver_preferences().add_receiver(s);

    // Simulation for 3 turns
    simulate(f, 3, [](Factory&, Time){}); // Empty report

    // After 3 turns, Storehouse should contain at least 1 package
    EXPECT_GE(s->cend() - s->cbegin(), 1);
}

// ============================================================================
//                                I/O TESTS
// ============================================================================

TEST(IOTest, LoadFactoryStructure) {
    std::string structure = 
        "LOADING_RAMP id=1 delivery-interval=3\n"
        "WORKER id=1 processing-time=2 queue-type=FIFO\n"
        "STOREHOUSE id=1\n"
        "LINK src=ramp-1 dest=worker-1\n"
        "LINK src=worker-1 dest=store-1\n";
        
    std::stringstream ss(structure);
    Factory f = load_factory_structure(ss);

    // Verify Objects existence
    ASSERT_NE(f.find_ramp_by_id(1), f.ramp_cend());
    ASSERT_NE(f.find_worker_by_id(1), f.worker_cend());
    ASSERT_NE(f.find_storehouse_by_id(1), f.storehouse_cend());

    // Verify properties
    auto& r = *f.find_ramp_by_id(1);
    EXPECT_EQ(r.get_delivery_interval(), 3);
    
    auto& w = *f.find_worker_by_id(1);
    EXPECT_EQ(w.get_processing_duration(), 2);

    // Verify Connections
    auto ramp_prefs = r.get_receiver_preferences().get_preferences();
    EXPECT_FALSE(ramp_prefs.empty());
    // Should point to worker #1
    EXPECT_EQ(ramp_prefs.begin()->first->get_id(), 1); 
}

TEST(IOTest, SaveFactoryStructure) {
    Factory f;
    f.add_ramp(Ramp(1, 3));
    f.add_worker(Worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    f.add_storehouse(Storehouse(1));

    // Links
    auto r = &(*f.find_ramp_by_id(1));
    auto w = &(*f.find_worker_by_id(1));
    auto s = &(*f.find_storehouse_by_id(1));
    r->get_receiver_preferences().add_receiver(w);
    w->get_receiver_preferences().add_receiver(s);

    std::stringstream ss;
    save_factory_structure(f, ss);
    
    std::string output = ss.str();

    // Check if output contains key definitions
    EXPECT_NE(output.find("LOADING_RAMP id=1 delivery-interval=3"), std::string::npos);
    EXPECT_NE(output.find("WORKER id=1 processing-time=2"), std::string::npos);
    EXPECT_NE(output.find("STOREHOUSE id=1"), std::string::npos);
    EXPECT_NE(output.find("LINK src=ramp-1 dest=worker-1"), std::string::npos);
    EXPECT_NE(output.find("LINK src=worker-1 dest=store-1"), std::string::npos);
}

// ============================================================================
//                                MAIN RUNNER
// ============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}