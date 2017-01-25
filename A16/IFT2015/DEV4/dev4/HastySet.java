
public class HastySet extends NLinearProbing {
	
	private static final float MIN_LOAD_FACTOR = (float)0.25;
	private final float min_load_factor;
	
	public HastySet(int initial_capacity, float max_load_factor, float min_load_factor) {
		super(initial_capacity, max_load_factor);
		this.min_load_factor = min_load_factor;
	}
	
	public HastySet(int initial_capacity) {
		this(initial_capacity, MAX_LOAD_FACTOR, MIN_LOAD_FACTOR);
	}
	
	public HastySet(){
		this(DEFAULT_CAPACITY);
	}
	
	@Override
    public boolean remove(Object emt)
    {
		Object o;
    	int i = search(emt);
        if (equals(table[i], emt))
        {
            table[i] = VOID;
            i = (i + 1) % table.length;
            while (table[i] != VOID) {
            	o = table[i];
            	table[i] = VOID;
            	add(o);
                i = (i + 1) % table.length;
            }
            --size;
            if (loadFactor() < min_load_factor && capacity_bits > 2)
                rehash(-1);
            return true;
        } else
            return false; 
    }
}
