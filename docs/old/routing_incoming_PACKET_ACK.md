## Locha routing - Algo for incoming packets type : PACKET_ACK

```mermaid
graph TD
A[routing_incoming_PACKET_ACK] --> B{TO=id_nodo}
B -->|No|D[reenviar paquete a los vecinos nivel 1 actualizando la metrica]
B -->|Si|C[verificar en el MSG el tipo de paquete original]
C -->J{Tipo de paquete original}
J -->|PACKET_HELLO| I[Actualizar tabla de vecinos]
I --> L[Enviar un PACKET NETJOIN con los service type en el MSG]
L -->K[Borrar de la cola de paquetes enviados en espera]
J -->|PACKET_MSG| K
K -->G[Salir]
D -->G[Salir]
```