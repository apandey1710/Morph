const expected = [0, 10, 13, 127, 128, 255];
const observations = {
  correct: expected,
  corrupt: [0, 10, 13, 127, 128, 0],
  short: [0, 10, 13],
  extra: [...expected, 66],
};
const scenario = document.querySelector("#scenario");
if (scenario) {
  const assertion = document.querySelector("#assertion");
  function render() {
    const actual = observations[scenario.value];
    const exact = actual.length === expected.length && actual.every((byte, i) => byte === expected[i]);
    const pass = assertion.value === "equal" ? exact : assertion.value === "size" ? actual.length === expected.length : actual.length > 0;
    document.querySelector("#observed").textContent = actual.map(byte => byte.toString(16).padStart(2, "0").toUpperCase()).join(" ");
    const result = document.querySelector("#lab-result");
    result.dataset.state = pass ? "pass" : "fail";
    result.textContent = `${pass ? "PASS" : "FAIL"} — ${pass && !exact ? "This assertion missed the bug. The observed bytes are wrong." : exact ? "The bytes match exactly." : "This assertion detected the incorrect contents."}`;
  }
  scenario.addEventListener("change", render);
  assertion.addEventListener("change", render);
  render();
}
for (const pre of document.querySelectorAll("pre[data-copy]")) {
  const button = document.createElement("button");
  button.type = "button";
  button.className = "copy-button";
  button.textContent = "Copy code";
  button.addEventListener("click", async () => {
    try {
      await navigator.clipboard.writeText(pre.querySelector("code").textContent);
      button.textContent = "Copied";
    } catch {
      button.textContent = "Select code to copy";
      const range = document.createRange();
      range.selectNodeContents(pre.querySelector("code"));
      const selection = window.getSelection();
      selection.removeAllRanges();
      selection.addRange(range);
    }
    setTimeout(() => { button.textContent = "Copy code"; }, 2500);
  });
  pre.append(button);
}
