function (obj) {
  const exp = obj.instance.exports;

  function repaint() {
    exp.casein_repaint();
    window.requestAnimationFrame(repaint);
  }

  exp.casein_create_window();
  window.requestAnimationFrame(repaint);

  document.addEventListener('keydown', (e) => exp.casein_keycode_down(e.keyCode));
  document.addEventListener('keyup', (e) => exp.casein_keycode_up(e.keyCode));
}
