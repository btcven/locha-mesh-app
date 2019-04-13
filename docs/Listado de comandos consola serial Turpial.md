El Turpial tiene la posibilidad de recibir comandos via consola serial cuando esta conectado a un puerto USB
La velocidad de conexion del puerto es 115200
Los comandos serial solo funcionan si en debugging.h esta sin comentar la linea #define DEBUG true
Los comandos serial pueden ser para mostrar valores o para ejecutar acciones en el turpial.
Los comandos que ejecutan acciones tambien pueden ser invocados via BLE usando el prefijo REMOTE:nombre_del_comando , no se devuelve ningun tipo de interaccion al movil ya que esta funcionalidad es solo para debugging


Lista de comandos disponibles via serial:

para nodos:
SHOW NODES            muestra los nodos en la tabla de nodos del sistema
NODE CREATE XXXX      crea un nodo virtual en la tabla de nodos
NODE SETNAME XXXX     cambia el nombre del nodo y vacia todas las tablas en memoria
SHOW BLACKLIST        muestra los nodos en la tabla de blacklisted
BLACKLIST NODE XXXX   coloca un nodo en blacklist

para rutas:
SHOW ROUTES     muestra la tabla de rutas

para paquetes y cola de paquetes:
SHOW QUEUE            muestra la cola de mensajes salientes que no han sido procesados
SHOW WAITING          muestra la cola de mensajes en espera (ya sea para reintento o esperando un ACK de retorno)
PACKET SHOW           muestra el contenido del Buffer_packet actualmente en memoria
PACKET DELETE QUEUE XXX       borra de la cola de paquetes salientes el que este identificado como xxx (usando el comando show queue)
PACKET DELETE WAITING XXX     borra de la cola de paquetes waiting el que este identificado como xxx (usando el comando show waiting)
PACKET CREATE INCOMING TYPE FROM PAYLOAD            crea un paquete incoming del tipo TYPE asignado, cuyo origen es FROM y el contenido es PAYLOAD
PACKET CREATE OUTCOMING TYPE TO PAYLOAD             crea un paquete outcoming del tupo TYPE asignado, cuyo destinatario es TO y el contenido es PAYLOAD
      

globales:
SYSTEM INFO            muestra el id unico del equipo
CLEAR ALL              vacia las tablas de mensajes salientes, nodos y rutas
SYSTEM RESET           reinicia el equipo

para la radio Lora:
MSG RADIO XXX          envia un mensaje XXX por el radio Lora


para BLE:
BLE CREATE INCOMING XXXX           asigna a la variable rxValue el valor XXXX
BLE CREATE OUTCOMING XXXX          asigna a la variable txValue el valor XXXX
BLE SHOW                           muestra el contenido de las variables rxValue y txValue
BLE CLEAR                          limpia el buffer BLE (rxValue y txValue)
BLE TEST                           envia por BLE un paquete de prueba de 30 caracteres
BLE INFO                           se devuelve al BLE el id_nodo conectado
