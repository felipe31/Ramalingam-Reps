FILES1=
FILES2=
FILES3=
CMP1=
CMP2=
CMP3=
INPUT=ger_in.txt

define clean_especific
	-rm ./$(CMP1)* ./$(CMP2)* ./cg.$(CMP2)* ./cg.$(CMP1)*
endef

define compare
	$(call clean_especific)
	gcc $(FILES1) -o $(CMP1) -g
	gcc $(FILES2) -o $(CMP2) -g
	bash -c 'time ./$(CMP1) < ./$(INPUT) > $(CMP1)_out.txt'
	bash -c 'time ./$(CMP2) < ./$(INPUT) > $(CMP2)_out.txt'
	@echo 'Comparing output of $(CMP1) and $(CMP2)'
	-diff ./$(CMP1)_out.txt ./$(CMP2)_out.txt
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP1) < ./$(INPUT)
	mv ./callgrind.out.* ./cg.$(CMP1)
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP2) < ./$(INPUT)
	mv ./callgrind.out.* ./cg.$(CMP2)
	kcachegrind cg.$(CMP1) cg.$(CMP2)  2>/dev/null 1>&2 &
endef

define compare3
	$(call clean_especific)
	gcc $(FILES1) -o $(CMP1) -g
	gcc $(FILES2) -o $(CMP2) -g
	gcc $(FILES3) -o $(CMP3) -g
	bash -c 'time ./$(CMP1) < ./$(INPUT) > $(CMP1)_out.txt'
	bash -c 'time ./$(CMP2) < ./$(INPUT) > $(CMP2)_out.txt'
	bash -c 'time ./$(CMP3) < ./$(INPUT) > $(CMP3)_out.txt'
	@echo 'Comparing output of $(CMP1) and $(CMP2) and $(CMP3)'
	-diff ./$(CMP1)_out.txt ./$(CMP2)_out.txt
	-diff ./$(CMP3)_out.txt ./$(CMP2)_out.txt
	-diff ./$(CMP1)_out.txt ./$(CMP3)_out.txt
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP1) < ./$(INPUT)
	mv ./callgrind.out.* ./cg.$(CMP1)
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP2) < ./$(INPUT)
	mv ./callgrind.out.* ./cg.$(CMP2)
	valgrind 1>/dev/null 2>&1 --tool=callgrind ./$(CMP3) < ./$(INPUT)
	mv ./callgrind.out.* ./cg.$(CMP3)
	kcachegrind cg.$(CMP1) cg.$(CMP2) cg.$(CMP3)  2>/dev/null 1>&2 &
endef


all:
	@echo 'None command'

bh: ../rr_binary_heap/rr.c ../rr_binary_heap/rr.h ../rr_binary_heap/rr-D.c
	@echo 'Comparing rr with vector binary heap and rr with dynamic binary heap'
	$(eval FILES1 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c ../rr_binary_heap/heap.c ../rr_binary_heap/graph.c)
	$(eval FILES2 := ../rr_binary_heap/rr-D.c ../rr_binary_heap/heap.c)
	$(eval CMP1 := rr_bh_F)
	$(eval CMP2 := rr_bh_D)
	$(call compare)

avl: ../rr_avl_olsr/rr.c ../rr_avl_olsr/rr.h ../rr-D.c
	@echo 'Comparing rr with avl from olsr and rr with dynamic binary heap'
	$(eval FILES1 := ../rr_avl_olsr/main.c ../rr_avl_olsr/rr.c ../rr_avl_olsr/heap.c ../rr_avl_olsr/graph.c ../rr_avl_olsr/avl.c)
	$(eval FILES2 := ../rr-D.c)
	$(eval CMP1 := rr_avl_F)
	$(eval CMP2 := rr_bh_D)
	$(call compare)


bh_avl:
	@echo 'Comparing rr with avl from olsr and rr with static binary heap'
	$(eval FILES1 := ../rr_avl_olsr/main.c ../rr_avl_olsr/rr.c ../rr_avl_olsr/heap.c ../rr_avl_olsr/graph.c ../rr_avl_olsr/avl.c)
	$(eval FILES2 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c ../rr_binary_heap/heap.c ../rr_binary_heap/graph.c)
	$(eval CMP1 := rr_avl_F)
	$(eval CMP2 := rr_bh_F)
	$(call compare)

olsr_bh_avl:
	@echo 'Comparing rr with avl from olsr and rr with dynamic binary heap from olsr'
	$(eval FILES1 := ../rr_avl_olsr/main.c ../rr_avl_olsr/rr.c ../rr_avl_olsr/heap.c ../rr_avl_olsr/graph.c ../rr_avl_olsr/avl.c)
	$(eval FILES2 := ../rr_bh_olsr/main.c ../rr_bh_olsr/rr.c ../rr_bh_olsr/heap.c ../rr_bh_olsr/graph.c ../rr_bh_olsr/heap_olsr.c)
	$(eval CMP1 := rr_avl_olsr)
	$(eval CMP2 := rr_bh_olsr)
	$(call compare)

bh_bh_olsr:
	@echo 'Comparing rr with bh from F and rr with dynamic binary heap from olsr'
	$(eval FILES1 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c ../rr_binary_heap/heap.c ../rr_binary_heap/graph.c)
	$(eval FILES2 := ../rr_bh_olsr/main.c ../rr_bh_olsr/rr.c ../rr_bh_olsr/heap.c ../rr_bh_olsr/graph.c ../rr_bh_olsr/heap_olsr.c)
	$(eval CMP1 := rr_bh_F)
	$(eval CMP2 := rr_bh_olsr)
	$(call compare)


bh2_avl:
	@echo 'Comparing rr with avl from olsr and rr with static binary heap'
	$(eval FILES1 := ../rr_avl_olsr/main.c ../rr_avl_olsr/rr.c ../rr_avl_olsr/heap.c ../rr_avl_olsr/graph.c ../rr_avl_olsr/avl.c)
	$(eval FILES2 := ../rr_binary_heap/main.c ../rr_binary_heap/rr.c ../rr_binary_heap/heap.c ../rr_binary_heap/graph.c)
	$(eval FILES3 := ../rr_bh_olsr/main.c ../rr_bh_olsr/rr.c ../rr_bh_olsr/heap.c ../rr_bh_olsr/graph.c ../rr_bh_olsr/heap_olsr.c)
	$(eval CMP1 := rr_avl_olsr)
	$(eval CMP2 := rr_bh_F)
	$(eval CMP3 := rr_bh_olsr)
	$(call compare3)


clean:
	-rm ./rr_* ./cg.* ./callgrind*
#	clear
#valgrind --tool=memcheck --leak-check=yes -v
