(function() {
  const canvas = document.createElement("canvas");
  canvas.id = "casein-canvas";
  document.body.appendChild(canvas);

  document.addEventListener('keydown', (e) => leco_exports.casein_key(1, e.keyCode));
  document.addEventListener('keyup', (e) => leco_exports.casein_key(0, e.keyCode));

  leco_imports.casein = {
    window_size : (x, y) => {
      canvas.style.width = x;
      canvas.style.height = y;
    },
    window_title : (x, y) => console.log(x, y),
  };
})();
