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

Storage logic:
1. Each storage can have its own storing logic


## APP FUNCTIONALITY
1. Load structure from a file, handle parsing errors
Input file structure:
- every line not being a comment or empty line describes one node or connection between nodes

ramp:
LOADING_RAMP id=<ramp-id> delivery-interval=<delivery-interval>

worker:
WORKER id=<worker-id> processing-time=<processing-time> queue-type=<queue-type>




## SIMULATION
Before simulation:
1. Net integrity check - all ramps have a final destination (storage connection)

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

