var gApImg = 0;
var canvas, ctx;
TEMPO_RELOAD_MIN = 150;
TEMPO_TIMER_PAINEL = 2500;
var gDataAtualizacao = null;
var gTimerPainel = null;

function inicio() {
    canvas = document.getElementById("telaCanvas");
    ctx = canvas.getContext('2d');
    setCam(1);
    gDataAtualizacao = Date.now();

}

function setCam(idFav) {
    /* define e inicia camera atual */
    showCam(idFav);
    playPainel();
}


function showCam(camera) {
        if (gApImg == 0) {
            // inicializa
            gApImg = 25;
        }
        gApImg += 1;
        if (gApImg > 25) {
            gApImg = 1;
        }
        url = "http://localhost/cam01/" + gApImg + ".jpg" + '?' + new Date().getTime();
    var img = new Image();
    img.onload = function () {
        ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
    }
    img.src = url;
}

function playPainel() {
    stopPainel();
    gTimerPainel = setInterval(fn_timerAtualizaPainel, TEMPO_TIMER_PAINEL);
}


function stopPainel() {
    if (gTimerPainel != null) {
        clearTimeout(gTimerPainel);
        gTimerPainel = null;
    }
}

function playStopPainel() {
    if (gTimerPainel == null) {
        playPainel();
    } else {
        stopPainel();
    }
}

function fn_timerAtualizaPainel() {
    /* timer que atualiza as imagens da camera atual */
    checkReload();
    showCam(1);
}

function checkReload() {
    var deltaMinutos = (Date.now() - gDataAtualizacao) / 1000 / 60.;
    if (deltaMinutos > TEMPO_RELOAD_MIN) {
        location.reload();
    }
}
