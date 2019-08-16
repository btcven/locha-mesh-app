## Locha routing - Algo for incoming packets type : PACKET_GOSSIP

```mermaid 
graph TD
A[routing_incoming_PACKET_GOSSIP] --> B{es un nodo vecino?}
B-->|Si|C[devolver otro paquete PACKET_GOSSIP al originante con el listado de services en el payload ]
B -->|No|D{3>Hops?}
D-->|No|E[reenviar a los vecinos de nivel1]
C -->H[Salir]
D-->|Si|H
E -->H
```
