/*
 * Memory Manage Unit
 * Protection: when process access invalid address, trigger a #GP
 * Paging: when process access a page not loaded in memory, trigger a #PF 
 */

/**
 * class CPU_mmu
 * Each instance of this class is a mmu of a core
 */
class CPU_mmu {
public:
    struct mmu_result {
        
	}
    void read(size_t la);
	void write(size_t la, char data);
private:
	
};
