
# The user can change the following on the command line:
PYTHON3BIN = python

SHELL = /bin/bash
.ONESHELL:  # each recipe is executed as a single script

.PHONY: all build-check installdeps clean

all: build-check

build-check:
	@set -euxo pipefail
	$(PYTHON3BIN) -m build --sdist
	dist_files=(dist/*.tar.gz)
	$(PYTHON3BIN) -m twine check --strict "$${dist_files[@]}"
	if [[ $${#dist_files[@]} -ne 1 ]]; then echo "More than one dist file:" "$${dist_files[@]}"; exit 1; fi
	PYTHON3BIN="$(PYTHON3BIN)" dev/isolated-dist-test.sh "$${dist_files[0]}"
	echo "$${dist_files[@]}"

installdeps:
	@set -euxo pipefail
	$(PYTHON3BIN) -m pip install --upgrade --upgrade-strategy=eager --no-warn-script-location pip
	$(PYTHON3BIN) -m pip install --upgrade --upgrade-strategy=eager --no-warn-script-location -r dev/requirements.txt

clean:
	@set -euxo pipefail
	git clean -dxf -e '.venv*'
