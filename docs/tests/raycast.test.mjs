import assert from "node:assert/strict";
import {
  level,
  origin,
  castRay,
  camera,
  cameraRay,
} from "../assets/raycast.mjs";
const near = (a, b) => assert.ok(Math.abs(a - b) < 1e-9, `${a} != ${b}`);
let result = castRay(level, origin, { x: 1, y: 0 });
assert.equal(result.hit.x, 7);
assert.equal(result.hit.y, 1);
near(result.hit.t, 4.5);
assert.equal(result.trace.length, 5);
near(castRay(level, origin, { x: 0, y: -1 }).hit.t, 0.5);
near(castRay(level, origin, { x: 2, y: 0 }).hit.t, 2.25);
result = castRay(level, origin, { x: 0.8, y: 0.6 });
assert.equal(result.hit.side, "Y");
near(result.hit.t, 5 / 6);
near(result.hit.position.x, 19 / 6);
assert.equal(castRay(level, origin, { x: 0, y: 0 }).hit, null);
assert.equal(castRay(level, origin, { x: Infinity, y: 0 }).hit, null);
assert.equal(castRay(level, { x: -0.1, y: 1 }, { x: 1, y: 0 }).hit, null);
assert.equal(castRay(level, { x: 0, y: 0 }, { x: 1, y: 0 }).hit, null);
assert.equal(
  castRay(
    Array.from({ length: 3 }, () => Array(5).fill(0)),
    origin,
    { x: 1, y: 0 },
  ).hit,
  null,
);
near(castRay(level, { x: 1, y: 1.5 }, { x: -1, y: 0 }).hit.t, 0);
const corner = [
  [0, 1, 0],
  [0, 1, 0],
  [0, 0, 0],
];
result = castRay(corner, { x: 0.5, y: 0.5 }, { x: 1, y: 1 });
assert.equal(result.hit.side, "X");
assert.equal(result.hit.y, 0);
for (let angle = -180; angle <= 180; angle += 15)
  for (const fov of [30, 60, 110]) {
    const { forward, plane } = camera(angle, fov);
    near(Math.hypot(forward.x, forward.y), 1);
    near(forward.x * plane.x + forward.y * plane.y, 0);
    for (let i = 0; i < 31; i++) {
      const d = cameraRay(forward, plane, (2 * (i + 0.5)) / 31 - 1),
        { hit } = castRay(level, origin, d);
      assert.ok(hit);
      assert.ok(hit.t >= 0);
      const h = { x: hit.position.x - origin.x, y: hit.position.y - origin.y };
      near(h.x * forward.x + h.y * forward.y, hit.t);
      near(Math.hypot(h.x, h.y), hit.t * Math.hypot(d.x, d.y));
    }
  }
const room = [
  [1, 1, 1, 1, 1],
  [1, 0, 0, 0, 1],
  [1, 0, 0, 0, 1],
  [1, 0, 0, 0, 1],
  [1, 1, 1, 1, 1],
];
const { forward, plane } = camera(0, 60);
for (let i = 0; i < 51; i++)
  near(
    castRay(
      room,
      { x: 2.5, y: 2.5 },
      cameraRay(forward, plane, (2 * (i + 0.5)) / 51 - 1),
    ).hit.t,
    1.5,
  );
console.log(
  "Browser DDA and camera checks passed (2,325 camera rays plus edge cases).",
);
