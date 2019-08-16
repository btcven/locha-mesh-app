## Locha routing - Algo for incoming packets type : PACKET_NETROUTE

```mermaid
graph TD
A[routing_incoming_NETROUTE] --> B{TO=Esta en la tabla de vecinos}
B-->|Si| C{TO=id_nodo}
C -->|Si|D[Actualizo tabla de rutas coloco ambos nodos como neighbor asignando al del from hops_away=1 y al del payload hops_away=2]
C -->|No|I[Actualizo tabla de rutas coloco en el from el id del nodo actual, max_hops+1 y el payload el ID del nodo previo de donde venia]
B -->|No|E[Actualizo la tabla de rutas como nodo no prioritario]
I -->F[reenviar paquete a los vecinos nivel 1 actualizando la metrica]
F -->H[Salir]
E -->H
D -->H
```