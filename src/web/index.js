/**
 * Loading ROM
 */
function uploadRom() {
    let fileInputElement = document.getElementById("romFile");
    fileInputElement.click();
}

function updateRomName(event) {
    let file = event.target.files[0];
    let fileName = file.name;
    let inputElement = document.getElementById("romName");
    inputElement.value = fileName;
}

let fileReader = new FileReader();

function loadRom() {
    let file = document.getElementById('romFile').files[0];
    fileReader.addEventListener('loadend', runRom);
    fileReader.readAsArrayBuffer(file);
}

async function runRom(e) {
    let result = fileReader.result;
    const resultByteArray = new Uint8Array(result);
    const memFsFilename = "rom.nes"
    FS.writeFile(memFsFilename, resultByteArray);
    await Module.ccall('loadRom', null, ['string'], [memFsFilename], { async: true });
}

/**
 * Module
 */
const CANVAS_ID = "canvas";

var Module = {
    onRuntimeInitialized: function() {
        Module.ccall('run', null, null, null, { async: false });
        let canvasContainer = this.canvas.parentElement;
        this.canvas.width = canvasContainer.clientWidth;
        this.canvas.height = canvasContainer.clientHeight;
    },
    
    canvas: (function(){
        let canvas = document.getElementById(CANVAS_ID);
        return canvas;
    })()
}