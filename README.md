# vein-framework
Vein framework is an event broker system. Data transferred is structured as entity with components.
## Entity/Component rules:
* Entities are identified by a system-unique integer id
* Entities are containers for components
* Entities should have a component 'EntityName'
* Components are identified by a component name which must be unique within its entity
* Component data is of type QVariant

## Framework
Framework contains all interfaces for events / event handler and storage system.<br>
[Source code](framework)
### Class diagram vein events:
![Class diagram events](framework/doc/class-diagram-events.svg)

## Common convenient code
Code to perform common operations in vein-framework.<br>
[Source code](convenient-code)
### Class diagram convenient code:
![Class diagram convenient code](convenient-code/doc/class-diagram.svg)

## Client libraries
[Source code](client-libs)

## Server libraries
[Source code](server-libs)

## Network
[Source code](network)
