SUBDIRS=src

default: subdirs csim.mexglx csim.dll

subdirs: $(SUBDIRS)
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i; done)

documentation:
	$(MAKE) -C documentation

csim.mexglx: src/csim.mexglx
	cp src/csim.mexglx csim.mexglx 

csim.dll:
	cp src/csim.dll csim.dll
        
mrproper:
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i mrproper; done)
	@rm -f csim.* install.m

clean: $(SUBDIRS)
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done)
	@rm -f csim.dll csim.mexglx install.m

distclean: $(SUBDIRS)
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i distclean; done)
	@rm -f *~
	@rm -f demos/*.mat
	@find . -name auto -exec rm -rf {} \;
