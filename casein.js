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
    leco_exports.casein_resize(canvas.clientWidth, canvas.clientHeight);
  });

  document.body.style.position = "relative";
  document.body.appendChild(canvas);

  var canvas_width  = canvas.clientWidth;
  var canvas_height = canvas.clientHeight;

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
        canvas_width  = canvas.clientWidth;
        canvas_height = canvas.clientHeight;
        canvas.width  = canvas.parentElement.clientWidth  - 2;
        canvas.height = canvas.parentElement.clientHeight - 2;
      } else {
        canvas.width  = canvas_width;
        canvas.height = canvas_height;
      }
    },
    window_size : (x, y) => {
      canvas.clientWidth = x;
      canvas.clientHeight = y;
    },
    window_title : (ptr, size) => document.title = vaselin_tostr(ptr, size),
  };
}();
