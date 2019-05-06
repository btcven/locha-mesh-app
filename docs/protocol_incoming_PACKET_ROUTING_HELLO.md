## Locha routing - Algo for incoming packets type : PACKET_HELLO

```mermaid
graph TD
A[routing_incoming_PACKET_HELLO] --> B{remitente esta en blacklist}
B -->|Si| G[Salir]
B -->|No| D{ruta/ruta inversa esta en blacklist}
D -->|No|E{es vecino}
E -->|Si| G
E-->|No|F(Crear vecino)
F-->H(Crear ruta)
H-->J(devolver packet ROUTING JOIN)
J-->G
D  -->|Si| G
```