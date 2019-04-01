$arduino_cli_version = "0.3.6-alpha.preview"
$url = "https://github.com/arduino/arduino-cli/releases/download/$arduino_cli_version/arduino-cli-$arduino_cli_version-windows.zip"
$output = "$PSScriptRoot\arduino-cli.zip"
$output_dir "$PSScriptRoot"
$start_time = Get-Date

### Install Chocolatey
Set-ExecutionPolicy Bypass -Scope Process -Force; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

### Install git with chocolatey
choco install git.install

### Download arduino-cli
$wc = New-Object System.Net.WebClient
$wc.DownloadFile($url, $output)
#OR
(New-Object System.Net.WebClient).DownloadFile($url, $output)

Write-Output "Time taken: $((Get-Date).Subtract($start_time).Seconds) second(s)"

Expand-Archive $output -DestinationPath $outpud_dir

### Install Libs of Locha and compile

./arduino-cli-$arduino_cli_version-windows.exe core update-index
./arduino-cli-$arduino_cli_version-windows.exe core install Heltec-esp32:esp32
git clone https://gitlab.com/btcven/locha/heltec-lora  arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-lora
git clone https://gitlab.com/btcven/locha/heltec-oled  arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-oled
./arduino-cli-$arduino_cli_version-windows.exe lib install Time
./arduino-cli-$arduino_cli_version-windows.exe compile --fqbn Heltec-esp32:esp32:wifi_lora_32_V2 Turpial