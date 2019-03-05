## Locha routing - Algo for incoming packets type : PACKET_MSG

```mermaid
graph TD
A[routing_outcoming_PACKET_MSG]  -->B{existe ruta al nodo to?}
B -->|Si|C[enviar por la ruta existente el PACKET_MSG]
B -->|No|D{existe una ruta a un nodo NODE_EXTENDED_CAPACITY}
E -->F{hizo ACK algun nodo router?}
D--> |No|E[enviar a cada nodo tipo ROUTER del nivel mayor un NET_ROUTE solicitando info de si conocen al destinatario]
F-->|Si| G[agregar ruta hacia nodo to en la tabla de rutas]
D-->|Si| I[reenviar packet al NODE_EXTENDED_CAPACITY]
F-->|No| K[colocar en la cola de espera y reintentar en X seg]
K -->L{alcanzo la cantidad maxima de reintentos?}
L-->|No| F
L-->|Si| M[marcar el packet como PACKET_NOTDELIVERED]
G -->C
I -->H
M -->H
C -->H[Salir]
```