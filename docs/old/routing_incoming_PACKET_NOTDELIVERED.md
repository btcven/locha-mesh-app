## Locha routing - Algo for incoming packets type : PACKET_NOTDELIVERED

```mermaid
graph TD
A[routing_incoming_PACKET_NOTDELIVERED]  -->B[actualizar en la cola de espera el packet_t y reintentar envio en XX segundos]
B -->C[marcar la cantidad de reenvios que lleva el paquete]
C -->H[Salir]
```