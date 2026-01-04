!function() {
  const canvas = document.createElement("canvas");
  canvas.id = "casein-canvas";
  canvas.style.border = "1px solid black";
  canvas.style.position = "absolute";
  canvas.style.top = "50%";
  canvas.style.left = "50%";
  canvas.style.transform = "translate(-50%, -50%)";

  function mevt(id, e) { leco_exports.casein_mouse(id, e.button, e.offsetX, e.offsetY); }

  const cro = new ResizeObserver(_ => {
    // offsetX counts border as well
    leco_exports.casein_resize(canvas.offsetWidth - 2, canvas.offsetHeight - 2);
  });

  document.body.style.position = "relative";
  document.body.appendChild(canvas);

  leco_imports.casein = {
    start_events : () => {
      canvas.addEventListener('mousedown', (e) => mevt(0, e));
      canvas.addEventListener('mousemove', (e) => mevt(1, e));
      canvas.addEventListener('mouseup', (e) => mevt(2, e));

      cro.observe(canvas);

      document.addEventListener('keydown', (e) => leco_exports.casein_key(1, e.keyCode));
      document.addEventListener('keyup', (e) => leco_exports.casein_key(0, e.keyCode));
    },
    set_fullscreen : (en) => {
      if (en) {
        canvas.style.width = canvas.style.height = '100%';
      } else {
        canvas.style.removeProperty("width");
        canvas.style.removeProperty("height");
      }
    },
    window_size : (x, y) => {
      canvas.width = x;
      canvas.height = y;
    },
    window_title : (ptr, size) => document.title = vaselin_tostr(ptr, size),
  };
}();
