import {
  level,
  origin as spawn,
  castRay,
  camera,
  cameraRay,
} from "./raycast.mjs";

const demo = document.querySelector("[data-demo]");
if (demo) {
  const mode = demo.dataset.demo;
  let origin = { ...spawn };
  const angle = demo.querySelector("[name=angle]");
  const fov = demo.querySelector("[name=fov]");
  const count = demo.querySelector("[name=count]");
  const incorrect = demo.querySelector("[name=incorrect]");
  const mapCanvas = demo.querySelector(".map-canvas");
  const viewCanvas = demo.querySelector(".view-canvas");
  let shown = Infinity;
  const point = (p) => ({ x: 40 + p.x * 60, y: 40 + p.y * 60 });
  function line(ctx, a, b, color, width = 1) {
    ctx.strokeStyle = color;
    ctx.lineWidth = width;
    ctx.beginPath();
    ctx.moveTo(a.x, a.y);
    ctx.lineTo(b.x, b.y);
    ctx.stroke();
  }
  function dot(ctx, p, r, color) {
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(p.x, p.y, r, 0, Math.PI * 2);
    ctx.fill();
  }
  function render() {
    const degrees = Number(angle.value),
      field = fov ? Number(fov.value) : 60;
    const { forward, plane } = camera(degrees, field);
    const central = castRay(level, origin, forward);
    const ctx = mapCanvas.getContext("2d");
    ctx.fillStyle = "#14282e";
    ctx.fillRect(0, 0, 560, 560);
    ctx.font = "12px monospace";
    ctx.textAlign = "center";
    for (let y = 0; y < 8; y++)
      for (let x = 0; x < 8; x++) {
        ctx.fillStyle = level[y][x] ? "#41585c" : "#1b3439";
        ctx.fillRect(40 + x * 60 + 1, 40 + y * 60 + 1, 58, 58);
      }
    ctx.fillStyle = "#b5c7c7";
    for (let i = 0; i < 8; i++) {
      ctx.fillText(i, 70 + i * 60, 26);
      ctx.fillText(i, 23, 75 + i * 60);
    }
    const trace = central.trace,
      progress = Math.min(shown, trace.length);
    if (mode === "single") {
      trace.slice(0, progress).forEach((c, i) => {
        ctx.fillStyle = c.wall
          ? c.side === "X"
            ? "#4b8d86"
            : "#ac6647"
          : "#326e66";
        ctx.fillRect(41 + c.x * 60, 41 + c.y * 60, 58, 58);
        ctx.fillStyle = "#ffffff";
        ctx.fillText(i + 1, 70 + c.x * 60, 75 + c.y * 60);
        dot(
          ctx,
          point(c.position),
          4,
          c.wall ? (c.side === "X" ? "#94e1d2" : "#f6b47d") : "#f6c387",
        );
      });
      if (progress)
        line(
          ctx,
          point(origin),
          point(trace[progress - 1].position),
          "#f6c387",
          2,
        );
      demo.querySelector(".trace-body").innerHTML = trace
        .map(
          (c, i) =>
            `<tr class="${i < progress ? "visited" : ""}"><td>${i + 1}</td><td>${c.side}</td><td>(${c.x}, ${c.y})</td><td>${c.t.toFixed(3)}</td><td>${c.wall ? "Wall" : "Empty"}</td></tr>`,
        )
        .join("");
      demo.querySelector(".step-state").textContent =
        `${progress} / ${trace.length} crossings revealed`;
    } else {
      const rays = count ? Number(count.value) : 41;
      for (let i = 0; i < rays; i++) {
        const ray = cameraRay(forward, plane, (2 * (i + 0.5)) / rays - 1);
        const { hit } = castRay(level, origin, ray);
        if (hit) {
          line(ctx, point(origin), point(hit.position), "#7bc6b677");
          dot(
            ctx,
            point(hit.position),
            2.5,
            hit.side === "X" ? "#94e1d2" : "#f6b47d",
          );
        }
      }
      const center = { x: origin.x + forward.x, y: origin.y + forward.y };
      const a = { x: center.x - plane.x, y: center.y - plane.y },
        b = { x: center.x + plane.x, y: center.y + plane.y };
      line(ctx, point(a), point(b), "#d9a9f2", 3);
      line(ctx, point(origin), point(a), "#d9a9f288");
      line(ctx, point(origin), point(b), "#d9a9f288");
    }
    line(
      ctx,
      point(origin),
      point({ x: origin.x + forward.x * 0.65, y: origin.y + forward.y * 0.65 }),
      "#fff5dc",
      3,
    );
    dot(ctx, point(origin), 6, "#fff5dc");
    if (viewCanvas) {
      const v = viewCanvas.getContext("2d"),
        w = viewCanvas.width,
        h = viewCanvas.height;
      v.fillStyle = "#243943";
      v.fillRect(0, 0, w, h / 2);
      v.fillStyle = "#152b2c";
      v.fillRect(0, h / 2, w, h / 2);
      const focal = w / 2 / Math.tan((field * Math.PI) / 360);
      for (let x = 0; x < w; x++) {
        const ray = cameraRay(forward, plane, (2 * (x + 0.5)) / w - 1),
          { hit } = castRay(level, origin, ray);
        if (!hit) continue;
        const depth =
          hit.t * (incorrect.checked ? Math.hypot(ray.x, ray.y) : 1);
        const height = focal / Math.max(depth, 0.001);
        v.fillStyle = hit.side === "X" ? "#94caba" : "#4b8d86";
        v.fillRect(
          x,
          Math.max(0, (h - height) / 2),
          1,
          Math.min(h, (h + height) / 2) - Math.max(0, (h - height) / 2),
        );
      }
      demo.querySelector(".projection-state").textContent = incorrect.checked
        ? "Ray length · deliberately incorrect"
        : "Camera depth · correct projection";
    }
    angle.nextElementSibling.value = `${degrees}°`;
    if (fov) fov.nextElementSibling.value = `${field}°`;
    if (count) count.nextElementSibling.value = count.value;
    const h = central.hit;
    mapCanvas.setAttribute(
      "aria-label",
      `Overhead map. Player at (${origin.x}, ${origin.y}), facing ${degrees} degrees. Rays end at the first wall.`,
    );
    demo.querySelector(".readout").textContent = h
      ? `Center ray → cell (${h.x}, ${h.y}) · ${h.side} side · t = ${h.t.toFixed(3)}`
      : "Ray left the map without a hit.";
  }
  demo.querySelectorAll("input").forEach((input) =>
    input.addEventListener("input", () => {
      shown = Infinity;
      render();
    }),
  );
  demo.querySelector("[data-next]")?.addEventListener("click", () => {
    const { forward } = camera(Number(angle.value), 60);
    const total = castRay(level, origin, forward).trace.length;
    shown = shown >= total ? 1 : shown + 1;
    render();
  });
  demo.querySelector("[data-restart]")?.addEventListener("click", () => {
    shown = 0;
    render();
  });
  demo.querySelector("[data-flat]")?.addEventListener("click", () => {
    origin = { x: 4.5, y: 4.5 };
    angle.value = 90;
    fov.value = 60;
    incorrect.checked = false;
    render();
  });
  demo.querySelector("[data-reset-camera]")?.addEventListener("click", () => {
    origin = { ...spawn };
    angle.value = 0;
    fov.value = 60;
    incorrect.checked = false;
    render();
  });
  render();
}

for (const pre of document.querySelectorAll("pre[data-copy]")) {
  const button = document.createElement("button");
  button.className = "copy-button";
  button.textContent = "Copy";
  button.setAttribute("aria-label", "Copy C++ example");
  button.addEventListener("click", async () => {
    try {
      await navigator.clipboard.writeText(
        pre.querySelector("code").textContent,
      );
      button.textContent = "Copied";
    } catch {
      button.textContent = "Select code to copy";
    }
    setTimeout(() => (button.textContent = "Copy"), 2000);
  });
  pre.append(button);
}
