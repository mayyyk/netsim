Precise - rounds logic
Solid - integrity verification
Scalable - polimorphism and design patterns

## Nodes logic
Product logic:
1. unique ID

Ramp logic:
1. Deliveries with set frequency 

Worker logic:
1. queuing appended products
2. processing product takes set amount of rounds
3. FIFO or LIFO processing, enable own preference in the future

storehouse logic:
1. Each storehouse can have its own storing logic


## APP FUNCTIONALITY
### 1. Load structure from a file, handle parsing errors
Input file structure:
- every line not being a comment (comments start with ";") or empty line describes one node or connection between nodes
- all valid lines in format: TAG {key=pair} x N (present N times)
- elements type appear in the file in the order: loading ramp, worker, storehouse, link


ramp:
LOADING_RAMP id=<ramp-id> delivery-interval=<delivery-interval>

worker:
WORKER id=<worker-id> processing-time=<processing-time> queue-type=<queue-type>
queue-type: preferred queue type e.g. FIFO, LIFO

storehouse:
STOREHOUSE id=<storehouse-id>

connection/link:
LINK src=<node-type>-<node-id> dest=<node-type>-<node-id>
node-type: node category e.g. ramp, worker, storehouse
node-id: node id in the given type (set in node-type)

### 2. Enable net structure modification (from the program level) and save new structure to a new file

### 3. Simulation run and reporting
Before simulation:
1. Net integrity check - all ramps have a final destination (storehouse connection)

Simulation start:
1. No changes to the net permitted

Round run:
1. Delivery
2. Handing - instant
3. Processing
4. Report - net structure OR simulation state -> .txt file

Simulation finish:
After all required rounds pass

Scalability and improvement mindset:
1. Workers weights for ramps

## Tasks

Michał Midor:
- application structure

TODO:
- business logic layer: products and their storage, nodes, manufacture, methods for building a net
- input file parsing
- data read and write layer
- report layer
- simulation
- testing

## Good practices
1. Const correctness
2. Code documentation
3. Compilation flags
4. Unit test (using Google Testing Framework) for non-trivial functions and methods
5. Using Doxygen

## Target User Group
1. Programmer (API user)
2. Analytic - runs program with net definition and get the report

clear public interface: net building, simulation run, report read

## File structure
