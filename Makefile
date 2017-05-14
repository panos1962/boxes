all:
	@(cd src && make --no-print-directory)

cleanup:
	@(cd src && make cleanup)

test:
	@make --no-print-directory all
	@(cd src && make --no-print-directory test)
