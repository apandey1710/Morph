"""Check local links/anchors and that the downloadable examples are current."""
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urlsplit
from zipfile import ZipFile, ZIP_DEFLATED
import sys

root = Path(__file__).resolve().parents[1]


class Page(HTMLParser):
    def __init__(self, path):
        super().__init__()
        self.ids = set()
        self.links = []
        self.feed(path.read_text())

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if 'id' in attrs:
            assert attrs['id'] not in self.ids, f'Duplicate id: {attrs["id"]}'
            self.ids.add(attrs['id'])
        for name in ('href', 'src'):
            if name in attrs:
                self.links.append(attrs[name])


pages = {path: Page(path) for path in root.glob('*.html')}
checked = 0
for path, page in pages.items():
    for link in page.links:
        url = urlsplit(link)
        if url.scheme or url.netloc:
            continue
        assert not url.path.startswith('/'), f'Project-prefix incompatible link: {link}'
        target = (path.parent / unquote(url.path)).resolve() if url.path else path
        assert target.is_file(), f'{path.name}: missing {link}'
        if url.fragment and target.suffix == '.html':
            assert unquote(url.fragment) in pages[target].ids, f'Missing anchor: {link}'
        checked += 1
examples = root / 'examples'
expected = {p.name for p in examples.iterdir()
            if p.suffix in ('.cpp', '.hpp', '.md') or p.name == 'CMakeLists.txt'}
if '--pack' in sys.argv:
    with ZipFile(examples / 'morph-testing.zip', 'w', ZIP_DEFLATED) as archive:
        for name in sorted(expected):
            archive.write(examples / name, name)
with ZipFile(examples / 'morph-testing.zip') as archive:
    assert set(archive.namelist()) == expected
    for name in expected:
        assert archive.read(name) == (examples / name).read_bytes(), f'Stale archive: {name}'
print(f'{len(pages)} pages, {checked} local links/anchors, and example archive verified.')
