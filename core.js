var HID = require('node-hid');
var activeWindows = require('electron-active-window');

function ExecPowershellCommand(command) {
    const { exec } = require('child_process');
    return new Promise(function (resolve) {
        exec(command, { 'shell': 'powershell.exe' }, (error, stdout, stderr) => {
            resolve(stdout);
        });
    });
}

async function OnReceiveDataFromHID(data) {
    if (data[2] == 1) {
        ExecPowershellCommand('Start https://www.facebook.com/');
    }
    if (data[2] == 2) {
        ExecPowershellCommand('Start https://www.twitter.com/');
    }
    if (data[2] == 3) {
        var clipboardText = await ExecPowershellCommand('Get-Clipboard');
        ExecPowershellCommand('Start "https://www.youtube.com/results?search_query=' + clipboardText + '"');
    }
    if (data[2] == 4) {
        ExecPowershellCommand('Start https://www.instagram.com/');
    }
    if (data[2] == 5) {
        var clipboardText = await ExecPowershellCommand('Get-Clipboard');
        ExecPowershellCommand('Start "https://www.google.com/search?q=' + clipboardText + '"');
    }
}


async function main() {

    var previousProfile = 0;

    var devices = HID.devices();
    var deviceInfo = devices.find(function (d) {
        var isGmmkPro = d.vendorId === 0x320F && d.productId === 0x5044;
        return isGmmkPro && d.usagePage === 0xFF60 && d.usage === 0x61;
    });

    if (deviceInfo) {
        var device = new HID.HID(deviceInfo.path);

        device.on("data", OnReceiveDataFromHID);

        i = 0;
        setInterval(() => {
            activeWindows().getActiveWindow().then((result) => {
                activeProgram = result.windowClass;

                activeProfile = 0x00;
                if (activeProgram.toLowerCase().includes("firefox")) {
                    activeProfile = 0x01;
                }
                else if (activeProgram.toLowerCase().includes("devenv")) {
                    activeProfile = 0x02;
                }
                else if (activeProgram.toLowerCase().includes("discord")) {
                    activeProfile = 0x03;
                }
                else if (activeProgram.toLowerCase().includes("explorer")) {
                    activeProfile = 0x04;
                }

                if (activeProfile != previousProfile) {
                    previousProfile = activeProfile;
                    device.write([0x00, 0x00, activeProfile]);
                }
            });
        }, 60)
    }
}

main();