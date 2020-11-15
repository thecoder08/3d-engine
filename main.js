var ctx = $('#canvas').getContext('2d');
var camx = 0;
var camy = 0;
var camz = 0;
var fov = 1;
var adown = false;
var sdown = false;
var ddown = false;
var wdown = false;
var udown = false;
var jdown = false;
var yoffset = $('#canvas').height/2;
var xoffset = $('#canvas').width/2;
function moveTo(x, y, z) {
  ctx.moveTo(xoffset + (fov * ((x + camx) / (z + camz))), yoffset + (fov * ((y + camy) / (z + camz))));
}
function lineTo(x, y, z) {
  ctx.lineTo(xoffset + (fov * ((x + camx) / (z + camz))), yoffset + (fov * ((y + camy) / (z + camz))));
}
function renderRectangularPrism() {
  moveTo(100, 0, 100);
  lineTo(100, 100, 100);
  lineTo(0, 100, 100);
  lineTo(0, 0, 100);
  lineTo(100, 0, 100);
  lineTo(100, 0, 0);
  moveTo(100, 100, 100);
  lineTo(100, 100, 0);
  moveTo(0, 0, 100);
  lineTo(0, 0, 0);
  moveTo(0, 100, 100);
  lineTo(0, 100, 0);
  lineTo(0, 0, 0);
  lineTo(100, 0, 0);
  lineTo(100, 100, 0);
  lineTo(0, 100, 0);
}
function renderTriangularPrism() {
  moveTo(-250, 250, 0);
  lineTo(-250, -100, 0);
  lineTo(-100, -100, 0);
  lineTo(-250, 250, 0);
  moveTo(-250, 250, 100);
  lineTo(-250, -100, 100);
  lineTo(-100, -100, 100);
  lineTo(-250, 250, 100);
  lineTo(-250, 250, 0);
  moveTo(-250, -100, 100);
  lineTo(-250, -100, 0);
  moveTo(-100, -100, 100);
  lineTo(-100, -100, 0);
}
document.onkeydown = function(event) {
  if (event.code == 'KeyS') {
    sdown = true;
  }
  if (event.code == 'KeyW') {
    wdown = true;
  }
  if (event.code == 'KeyD') {
    ddown = true;
  }
  if (event.code == 'KeyA') {
    adown = true;
  }
  if (event.code == 'KeyJ') {
    jdown = true;
  }
  if (event.code == 'KeyU') {
    udown = true;
  }
}
document.onkeyup = function(event) {
  if (event.code == 'KeyS') {
    sdown = false;
  }
  if (event.code == 'KeyW') {
    wdown = false;
  }
  if (event.code == 'KeyD') {
    ddown = false;
  }
  if (event.code == 'KeyA') {
    adown = false;
  }
  if (event.code == 'KeyJ') {
    jdown = false;
  }
  if (event.code == 'KeyU') {
    udown = false;
  }
}
$('#fov').oninput = function() {
  fov = this.value;
}
setInterval(function() {
  if (wdown) {
    camz--;
  }
  if (adown) {
    camx++;
  }
  if (sdown) {
    camz++;
  }
  if (ddown) {
    camx--;
  }
  if (jdown) {
    camy--;
  }
  if (udown) {
    camy++;
  }
  $('#canvas').width = $('#canvas').width;
  renderRectangularPrism();
  renderTriangularPrism();
  ctx.stroke();
});
