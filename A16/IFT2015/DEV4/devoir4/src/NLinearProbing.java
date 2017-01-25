
public class NLinearProbing extends LinearProbing {
	
	protected static final Object VOID = new Object();
	
	public NLinearProbing(int initial_capacity, float max_load_factor) {
		super();
		for (int i = 0; i < table.length; i++) {
			table[i] = VOID;
		}
	}
	
	public NLinearProbing(int initial_capacity)
    {
        this(initial_capacity, MAX_LOAD_FACTOR);
    }
	
	public NLinearProbing() {
		this(DEFAULT_CAPACITY);
	}

	protected int search(Object key) {
		int h = hashCode(key);
        int i = getTableIndex(h);
        while (table[i] != VOID && !equals(table[i], key))
            i = (i + 1) % table.length;
        return i;
	}
	
	@Override
    public boolean contains(Object emt)
    {
        if (emt == VOID) return false;
        int i = search(emt);
        return table[i] != VOID;
    }
	
	/**
     * Insertion of a new element.
     * 
     * @param emt element to be added
     * @return true if no equal element was on the table yet
     */
    @Override
    public boolean add(Object emt)
    {
        int i = search(emt);
        if (table[i] == VOID)
        {
            table[i] = emt;
            ++size;
            if (loadFactor() > max_load_factor)
                rehash(1);
            return true;
        } else
            return false;        
    }
    
    
    
    protected void rehash(int capacity_bits_delta)
    {
        int newcapbits = this.capacity_bits + capacity_bits_delta;
        Object[] old_table = this.table;
        this.table = new Object[1 << newcapbits];
        for (int i = 0; i < this.table.length; i++) table[i] = VOID;
        this.capacity_bits = newcapbits;
        this.size = 0;
        for (int i = 0; i < old_table.length; i++)
        {
            if (old_table[i] != VOID)
            {
                Object E = old_table[i];
                int j = search(E); 
                assert (table[j] == VOID); // all keys are distinct in old_table[]
                table[j] = E;
                size++;
            }
        }
    }
	
	protected boolean equals(Object A, Object B) {
		if (A == null) return B == null;
		return A.equals(B);
	}
	
	protected int hashCode(Object obj) {
		if (obj == null) return 0;
		return obj.hashCode();
	}
}
