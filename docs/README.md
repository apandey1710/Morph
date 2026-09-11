# Morph tutorial site

A dependency-free, pre-rendered GitHub Pages notebook. Start at `index.html`.
The three lessons cover DDA, camera-plane sampling, and wall projection.
The engine source is independent of these teaching examples.

## Preview locally

From the repository root:

```sh
python3 -m http.server 4173 --directory docs
```

Open http://localhost:4173. No Node packages, site generator, or external fonts
are required. JavaScript powers only the interactive labs and copy buttons;
lesson text, navigation, and downloads remain available without it.

## Publish with GitHub Pages

In the repository's Settings → Pages, set the build and deployment source to
**GitHub Actions**. Commit and push the site and the Pages workflow to `main`,
or run **Publish tutorial notebook** manually from the Actions tab after the
workflow is on GitHub. The workflow uploads `docs` and deploys it to Pages.

The expected project-site address is https://apandey1710.github.io/Morph/.
This is the configured repository's expected address, not a claim that the
site has already been deployed. All internal links are relative so the site
also works below the `/Morph/` project prefix.

Official setup reference:
https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages

## Editing

Edit the HTML pages directly; there is no generation step for deployment.
Shared styles are in `assets/style.css`. The pure browser math is in
`assets/raycast.mjs`; drawing and controls are in `assets/site.mjs`.
The examples follow the project's C++20, raylib, and Allman brace conventions.
Keep displayed snippets synchronized with the downloadable examples.

After changing example files, rebuild the downloadable archive from the
repository root:

```sh
python3 -c 'from pathlib import Path; from zipfile import ZipFile, ZIP_DEFLATED; p=Path("docs/examples"); z=ZipFile(p/"morph-tutorials.zip", "w", ZIP_DEFLATED); [z.write(f, f.name) for f in sorted(p.iterdir()) if f.suffix in (".cpp", ".hpp") or f.name == "CMakeLists.txt"]; z.close()'
```

## Check the math and examples

```sh
python3 docs/tests/check_site.py
node docs/tests/raycast.test.mjs
cmake -S docs/examples -B /tmp/morph-tutorial-build
cmake --build /tmp/morph-tutorial-build --parallel 2
c++ -std=c++20 -I/tmp/morph-tutorial-build/_deps/raylib-src/src \
    docs/tests/raycaster.cpp -o /tmp/morph-dda-test
/tmp/morph-dda-test
```

The C++ tests need raylib's header but do not open a window. Each example
executable supports Left / Right to rotate. Lesson 3 adds F to compare camera
depth with the intentionally incorrect ray-length projection.

For manual browser checks, visit all four pages, rotate each lab, change FOV
and ray count, step and restart the single-ray traversal, and toggle the
projection error. Use the flat-wall preset: the south wall should be flat with camera
depth and bowed with ray length. Also inspect at a narrow mobile width, use
Tab to reach controls, follow lesson links, and download the examples archive.
