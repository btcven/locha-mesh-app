## First Boot

The fist boot it's a initial stage of bootstraping, thant one lora mesh device have to do.

This stage it's about to set the basic configuration settings for the use of device on the mesh network

In the next image you can view the step of this process

The process:

```mermaid
sequenceDiagram
    PowerOn->>+Battery: Good Level ?
    alt is yes
       Battery->>PowerOn: Good Level
    else is no
       Battery-->>-PowerOn: PowerOff
    end
    Note over PowerOn,SCR: Chek NVR prev setup, if yes load if not continue to default conf
    Note over PowerOn,SCR: This for all Stage Init
    PowerOn->>SCR: Enabled SCR ?
    SCR-->>PowerOn: Yes
    PowerOn--xSCR: S0 Init
    loop
        SCR-->>SCR: Stage 0
    end
    PowerOn->>BLE: Enabled BLE ?
    BLE-->>PowerOn: Yes
    PowerOn--xBLE: S1 Init
    loop
        BLE-->>BLE: Stage 1
    end
    PowerOn->>WAP: Enabled WAP ?
    WAP-->>PowerOn: Yes
    PowerOn--xWAP: S2 Init
    loop
        WAP-->>WAP: Stage 2
    end
    PowerOn->>WST: Enabled WST ?
    WST-->>PowerOn: Yes
    PowerOn--xWST: S3 Init
    loop
        WST-->>WST: Stage 3
    end
    PowerOn->>RAD: Enabled RAD ?
    RAD-->>PowerOn: Yes
    PowerOn--xRAD: S4 Init
    loop
        RAD-->>RAD: Stage 4
    end
```