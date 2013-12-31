SUBDIRS=reference usermanual

default: subdirs

subdirs: $(SUBDIRS)
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i; done)


clean:
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done)

distclean:
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i distclean; done)
	@rm -rf auto WARNINGS commonstuff.tex

texclean:
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i texclean; done)

mrproper:
	@(for i in $(SUBDIRS); do $(MAKE) -C $$i mrproper; done)
	@rm -rf auto

