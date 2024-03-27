# vein-framework
Vein framework is an event broker system. Data transferred is structured as entities with components:

* Entities are identified by a system-unique integer id
* Entities are containers for components
* Entities should have a component 'EntityName'
* Components are identified by a component name which must be unique within its entity
* Component data is of type QVariant

<br>Code is divided into the following blocks:

## Framework ([framework](framework))
Framework contains all interfaces for events / event handler and storage system.<br>

### Events:
![Class diagram events](framework/doc/class-diagram-events.svg)

### Event-handler and event-systems:
![Class diagram event-handler](framework/doc/class-diagram-event-systems.svg)
---
<br>

## Common convenient code ([convenient-code](convenient-code))
Code to perform common operations in vein-framework.<br>
### Event handling classes:
![Class diagram convenient code](convenient-code/doc/class-diagram.svg)
---
<br>

## Client libraries ([client-libs](client-libs))
### Event-systems:
![Client event systems](client-libs/doc/class-diagram-client-event-systems.svg)
---
<br>

## Server libraries ([server-libs](server-libs))
### Event-systems:
![Server event systems](server-libs/doc/class-diagram-server-event-systems.svg)
---
<br>

## Network ([network](network))
### Event-systems:
![Class diagram network systems](network/doc/class-diagram-network-event-systems.svg)
---
<br>

