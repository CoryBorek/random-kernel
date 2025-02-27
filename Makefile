.PHONY: clean headers build iso qemu run

build:
	./scripts/build.sh

clean:
	./scripts/clean.sh

headers:
	./scripts/headers.sh

iso:
	./scripts/iso.sh

qemu:
	./scripts/qemu.sh

run: clean headers qemu