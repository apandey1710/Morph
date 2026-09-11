export const level = [
  [1, 1, 1, 1, 1, 1, 1, 1],
  [1, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 1, 0, 0, 0, 1],
  [1, 0, 0, 1, 0, 1, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 1],
  [1, 1, 1, 1, 1, 1, 1, 1],
];
export const origin = { x: 2.5, y: 1.5 };
export function castRay(map, p, d) {
  const h = map.length,
    w = map[0].length,
    trace = [];
  if (
    ![p.x, p.y, d.x, d.y].every(Number.isFinite) ||
    (!d.x && !d.y) ||
    p.x < 0 ||
    p.y < 0 ||
    p.x >= w ||
    p.y >= h
  )
    return { hit: null, trace };
  let x = Math.floor(p.x),
    y = Math.floor(p.y);
  if (map[y][x]) return { hit: null, trace };
  const sx = d.x < 0 ? -1 : 1,
    sy = d.y < 0 ? -1 : 1;
  const dx = d.x === 0 ? Infinity : Math.abs(1 / d.x);
  const dy = d.y === 0 ? Infinity : Math.abs(1 / d.y);
  let nx = d.x === 0 ? Infinity : (d.x < 0 ? p.x - x : x + 1 - p.x) * dx;
  let ny = d.y === 0 ? Infinity : (d.y < 0 ? p.y - y : y + 1 - p.y) * dy;
  for (let n = 0; n < w + h; n++) {
    let t, side;
    if (nx <= ny) {
      t = nx;
      nx += dx;
      x += sx;
      side = "X";
    } else {
      t = ny;
      ny += dy;
      y += sy;
      side = "Y";
    }
    if (x < 0 || y < 0 || x >= w || y >= h) return { hit: null, trace };
    const crossing = {
      x,
      y,
      t,
      side,
      position: { x: p.x + t * d.x, y: p.y + t * d.y },
      wall: map[y][x] !== 0,
    };
    trace.push(crossing);
    if (crossing.wall) return { hit: crossing, trace };
  }
  return { hit: null, trace };
}
export function camera(angleDegrees, fovDegrees) {
  const a = (angleDegrees * Math.PI) / 180,
    scale = Math.tan((fovDegrees * Math.PI) / 360);
  const forward = { x: Math.cos(a), y: Math.sin(a) };
  return { forward, plane: { x: -forward.y * scale, y: forward.x * scale } };
}
export function cameraRay(forward, plane, u) {
  return { x: forward.x + u * plane.x, y: forward.y + u * plane.y };
}
