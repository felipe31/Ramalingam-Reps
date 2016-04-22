FILES1=
FILES2=
CMP1=
CMP2=

define clean_especific
	-rm ./$(CMP1)* ./$(CMP2)* ./cg.$(CMP2)* ./cg.$(CMP1)*
endef

define compare
	$(call clean_especific)
	gcc $(FILES1) -o $(CMP1) -g
	gcc $(FILES2) -o $(CMP2) -g
	bash -c 'time ./$(CMP1) < ./ger_in.txt > $(CMP1)_out.txt'
	bash -c 'time ./$(CMP2) < ./ger_in.txt > $(CMP2)_out.txt'
	@echo 'Comparing output of $(CMP1) and $(CMP2)'
	-diff ./$(CMP1)_out.txt ./$(CMP2)_out.txt
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP1) < ./ger_in.txt
	mv ./callgrind.out.* ./cg.$(CMP1)
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP2) < ./ger_in.txt
	mv ./callgrind.out.* ./cg.$(CMP2)
	kcachegrind cg.$(CMP1) cg.$(CMP2)  2>/dev/null 1>&2 &
endef



all:
	@echo 'None command'

bh: ../rr_binary_heap/rr.c ../rr_binary_heap/rr.h ../rr-D.c
	@echo 'Comparing rr with vector binary heap and rr with dynamic binary heap'
	$(eval FILES1 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c)
	$(eval FILES2 := ../rr-D.c)
	$(eval CMP1 := rr_bh_F)
	$(eval CMP2 := rr_bh_D)
	$(call compare)

avl: ../rr_avl_olsr/rr.c ../rr_avl_olsr/rr.h ../rr-D.c
	@echo 'Comparing rr with avl from olsr and rr with dynamic binary heap'
	$(eval FILES1 := ../rr_avl_olsr/main.c ../rr_avl_olsr/rr.c ../rr_avl_olsr/avl.c)
	$(eval FILES2 := ../rr-D.c)
	$(eval CMP1 := rr_avl_F)
	$(eval CMP2 := rr_bh_avl_D)
	$(call compare)


clean:
	rm ./rr_* ./cg.* ./callgrind*
#	clear
