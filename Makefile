ADDON = build/Release/gadu.node
REPORTER = dot

$(ADDON): src/*.cc
	npm install

test: $(ADDON)
	@./node_modules/.bin/mocha \
		--reporter $(REPORTER) \
		--ui exports \
		tests/*.test.js

clean:
		rm -rf build