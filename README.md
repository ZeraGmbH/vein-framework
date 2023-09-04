# vein-framework
Vein framework is an event broker system. Data transferred is structured as entity with components:

* Entities are identified by a system-unique integer id
* Entities are containers for components
* Entities should have a component 'EntityName'
* Components are identified by a component name which must be unique within its entity
* Component data is of type QVariant

<br>Code is divided into the following blocks:
## Framework ([framework](framework))
Framework contains all interfaces for events / event handler and storage system.<br>

### Class diagram vein events:
![Class diagram events](framework/doc/class-diagram-events.svg)

## Common convenient code ([convenient-code](convenient-code))
Code to perform common operations in vein-framework.<br>
### Class diagram convenient code:
![Class diagram convenient code](convenient-code/doc/class-diagram.svg)

## Client libraries ([client-libs](client-libs))

## Server libraries ([server-libs](server-libs))

## Network ([network](network))

