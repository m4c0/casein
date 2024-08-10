!function() {
  const canvas = document.createElement("canvas");
  canvas.id = "casein-canvas";
  canvas.style.border = "1px solid black";
  canvas.style.position = "absolute";
  canvas.style.top = "50%";
  canvas.style.left = "50%";
  canvas.style.transform = "translate(-50%, -50%)";

  document.body.style.position = "relative";
  document.body.appendChild(canvas);

  document.addEventListener('keydown', (e) => leco_exports.casein_key(1, e.keyCode));
  document.addEventListener('keyup', (e) => leco_exports.casein_key(0, e.keyCode));

  leco_imports.casein = {
    window_size : (x, y) => {
      canvas.style.width = x;
      canvas.style.height = y;
    },
    window_title : (ptr, size) => document.title = vaselin_tostr(ptr, size),
  };
}();
