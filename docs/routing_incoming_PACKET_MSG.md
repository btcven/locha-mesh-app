## Locha routing - Algo for incoming packets type : PACKET_MSG

```mermaid
graph TD
A[routing_incoming_PACKET_MSG] --> B{TO=id_nodo}
B -->|No|C{existe ruta previa al nodo?}
B -->|Si|J[ejecutar procesar_paquete]
J -->K[devolver un paquete PACKET_ACK al from]
C -->|Si|E[reenviar paquete por la ruta previa]
C -->|No|F{existe ruta a un nodo EXTENDED CAPACITY?}
F -->|Si|G[reenviar paquete a un nodo de EXTENDED  CAPACITY]
F -->|Si|I[reenviar paquete al originante como PACKET_NOTDELIVERED]
E -->H[Salir]
G -->H
I -->H
K -->H
```