## Locha routing - Algo for incoming packets type : PACKET_HELLO

```mermaid
graph TD
A[routing_incoming_PACKET_HELLO] --> B(Cargar a la tabla de nodos vecinos)
B --> C{3>Metrica}
C -->|Si| G[Salir]
C -->|No| D{metrica=1}
D -->|No|H[enviar PACKET_HELLO del nodo origen a los vecinos que tengan metrica 1]
D  -->|Si| F[enviar PACKET_ACK al nodo origen colocando MSG el tipo de paquete original y actuqlizando los Hops]
F -->G
H -->G
```