/*
 * The MIT License
 *
 * Copyright 2016 Mikl&oacute;s Cs&#369;r&ouml;s.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


import java.util.AbstractSet;
import java.util.Iterator;
import java.util.Set;

/**
 *
 * Basic implementation of open addressing with linear probing and no deletion.
 * 
 * @author Mikl&oacute;s Cs&#369;r&ouml;s
 */
/// !AVERTISSEMENT: LA CLASSE A ÉTÉ MODIFIÉE!
/// Les seules modifications apportées sont le changement de quelques "private" en "protected"
/// Le comportement de classe (le code à l'intérieur des méthodes) n'as pas été affecté d'aucune manière
public class LinearProbing extends AbstractSet<Object> implements Set<Object>
{
    private static final int HASH_MULTIPLY =
            (int)((3.0-Math.sqrt(5))*(1<<31));
    // g = (sq5-1)/2 > 0.5 
    //
    // two's complement: 2^32-x = g * 2^32 
    // x= (1-g)*2^32 = (3-sq5)/2 * 2^32
    
    protected static final float MAX_LOAD_FACTOR = 0.66f;
    protected static final int DEFAULT_CAPACITY = 1<<10; // 1024
    
    public LinearProbing()
    {
        this(DEFAULT_CAPACITY); 
    }
    
    public LinearProbing(int initial_capacity)
    {
        this(initial_capacity, MAX_LOAD_FACTOR);
    }
    
    /**
     * Instantiation with given initial capacity and load factor threshold.
     * 
     * @param initial_capacity
     * @param max_load_factor 
     */
    public LinearProbing(int initial_capacity, float max_load_factor)
    {
        this.max_load_factor = max_load_factor;
        int b = 1;
        int cap = 1<<b;
        while (cap<initial_capacity)
        {
            cap = cap*2;
            b++;
        }
        this.table = new Object[cap];
        this.capacity_bits = b;

        
    }
    
    /**
     * Table capacity is double when this threshold is reached.
     */
    protected final float max_load_factor;
    
    /**
     * Table for storing the elements (using open addressing).
     */
    protected Object[] table;
    /**
     * Number of elements in the table.
     */
    protected int size;
    
    /**
     * Number of bits in the hash keys.
     */
    protected int capacity_bits;
    
    /**
     * Multiplicative hashing [Knuth TAO vol. III, 6.4]
     * 
     * @param x argument for hash function
     * @return Hash value between 0 and 2<sup>{@link #capacity_bits}</sup>-1
     */
    protected int getTableIndex(int x)
    {
        int idx = (HASH_MULTIPLY*x) >>> (32-capacity_bits); 
        return idx;
    }
    
    @Override
    public boolean isEmpty()
    {
        return size==0;
    }
    
    /**
     * Table size.
     * 
     * @return number of elements in the table.
     */
    @Override
    public int size()
    {
        return this.size;
    }

    protected float loadFactor()
    {
        return ((float)size) / table.length;
    }

    /**
     * Search for an element.
     * 
     * @param key query
     * @return index in the table where found,, or where it should be placed on insertion
     */
    protected int search(Object key)
    {
        int h = key.hashCode();
        int i = getTableIndex(h);
        while (table[i]!= null && !table[i].equals(key))
            i=(i+1) % table.length;
        return i;
    }
    
    @Override
    public boolean contains(Object emt)
    {
        if (emt==null) return false;
        
        int i = search(emt);
        return (table[i]!=null);
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
        if (emt==null) throw new UnsupportedOperationException("Cannot add null element in this implementation.");
        
        int i = search(emt);
        if (table[i]==null)
        {
            table[i]=emt;
            ++size;
            if (loadFactor()>max_load_factor)
                rehash(1);
            return true;
        } else
            return false;        
    }
    
    @Override
    public void clear()
    {
        LinearProbing tabula_rasa = new LinearProbing();
        this.table = tabula_rasa.table;
        this.size = 0;
    }
    

    /**
     * Deletion is not supported
     * @param does_not_matter
     * @return 
     * @throws UnsupportedOperationException 
     */
    @Override
    public boolean remove(Object does_not_matter)
    {
        throw new UnsupportedOperationException("Deletion is not supported in this implementation.");
    }
    
    /**
     * Reallocates the table. The argument specifies the difference between the 
     * current capacity buts and the new value. 
     * 
     * @param capacity_bits_delta +1 for doubling, -1 for halving
     */
    private void rehash(int capacity_bits_delta)
    {
        int newcapbits = this.capacity_bits+capacity_bits_delta;
        Object[] old_table = this.table;
        this.table = new Object[1<<newcapbits];
        this.capacity_bits = newcapbits;
        this.size = 0;
        for (int i=0; i<old_table.length; i++)
        {
            if (old_table[i] != null)
            {
                Object E = old_table[i];
                int j = search(E); 
                assert (table[j]==null); // all keys are distinct in old_table[]
                table[j] = E;
                size++;
            }
        }
    }

    /**
     * Iterator as per specification of {@link AbstractSet}.
     * 
     * @return iterator over the elements
     */
    @Override
    public Iterator<Object> iterator() 
    {
        return new Iterator<Object>()
        {
            final CellIndexIterator iter = new CellIndexIterator();
            
            @Override
            public boolean hasNext() 
            {
                return iter.hasNext();
            }

            @Override
            public Object next() 
            {
                int i = iter.next();
                return table[i];
            }
        };
    }

    
    /**
     * Iterator over indices of occupied cells. 
     */
    private class CellIndexIterator implements Iterator<Integer>
    {
        private int current_idx;
        CellIndexIterator()
        {
            current_idx=0;
            forwardToNextEmpty();
        }
        
        private void forwardToNextEmpty()
        {
            while (current_idx<table.length && table[current_idx]==null)
            {
                current_idx++;
            }
        }
        
        @Override
        public boolean hasNext()
        {
            return current_idx < table.length;
        }
        
        @Override
        public Integer next()
        {
            int j=current_idx;
            current_idx++;
            forwardToNextEmpty();
            return j;
        }
    }    
}