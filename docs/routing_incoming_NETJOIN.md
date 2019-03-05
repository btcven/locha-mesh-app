## Locha routing - Algo for incoming packets type : PACKET_NETJOIN

```mermaid
graph TD
A[routing_incoming_NETJOIN] 

A -->B[sacar los service type del MSG]
B -->C[Actualizar los services tabla de vecinos]
C--> D{TO=id_nodo}
D -->|No|E[reenviar paquete a los vecinos nivel 1 actualizando la metrica]
D -->|Si|H
E -->H[Salir]
```