
public class LazySet extends NLinearProbing {
	
	private static final Object DELETED = new Object();
	private static final float MIN_LOAD_FACTOR = (float)0.25;
	private final float min_load_factor;
	
	public LazySet(int initial_capacity, float max_load_factor, float min_load_factor) {
		super(initial_capacity, max_load_factor);
		this.min_load_factor = min_load_factor;
	}
	
	public LazySet(int initial_capacity) {
		this(initial_capacity, MAX_LOAD_FACTOR, MIN_LOAD_FACTOR);
	}
	
	public LazySet(){
		this(DEFAULT_CAPACITY);
	}
	
	private int searchDel(Object key) {
		int h = hashCode(key);
        int i = getTableIndex(h);
        while (table[i] != DELETED && table[i] != VOID && !equals(table[i], key))
            i = (i + 1) % table.length;
        return i;
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
        	i = searchDel(emt);
            table[i] = emt;
            ++size;
            if (loadFactor() > max_load_factor)
                rehash(1);
            return true;
        } else
            return false;        
    }
    
    
    @Override
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
            if (old_table[i] != VOID && old_table[i] != DELETED)
            {
                Object E = old_table[i];
                int j = search(E); 
                assert (table[j] == VOID); // all keys are distinct in old_table[]
                table[j] = E;
                size++;
            }
        }
    }
	
    @Override
    public boolean remove(Object emt)
    {
    	int i = search(emt);
        if (equals(table[i], emt))
        {
            table[i] = VOID;
            --size;
            if (loadFactor() < min_load_factor && capacity_bits > 2)
                rehash(-1);
            return true;
        } else
            return false; 
    }
    
}
