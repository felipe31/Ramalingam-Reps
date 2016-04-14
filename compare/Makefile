FILES1=
FILES2=
CMP1=
CMP2=

define compare
	gcc $(FILES1) -o $(CMP1) -g
	gcc $(FILES2) -o $(CMP2) -g
	bash -c 'time ./$(CMP1) > $(CMP1)_out.txt'
	bash -c 'time ./$(CMP2) > $(CMP2)_out.txt'
	@echo 'Comparing output of $(CMP1) and $(CMP2)'
	diff ./rr_bh_F_out.txt ./rr_bh_D_out.txt
	valgrind --tool=callgrind ./$(CMP1)
	mv callgrind.out.* cg.$(CMP1)
	valgrind --tool=callgrind ./$(CMP2)
	mv callgrind.out.* cg.$(CMP2)
	kcachegrind cg.* 2>/dev/null 1>&2 &
endef

all:
	@echo 'None command'

bh: ../rr_binary_heap/rr.c ../rr_binary_heap/rr.h ../
	@echo 'Comparing rr with vector binary heap and rr with dynamic binary heap'
	$(eval FILES1 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c ../rr_binary_heap/rr.h)
	$(eval FILES2 := ../rr-D.c)
	$(eval CMP1 := rr_bh_F)
	$(eval CMP2 := rr_bh_D)
	$(call compare)


clean:
	rm ./rr_* ./cg.*
#	clear