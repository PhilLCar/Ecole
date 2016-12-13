
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


import java.util.HashSet;
import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Random;
import java.util.Set;

/**
 *
 * Class for simulating a dynamically changing set. 
 * When the set has <var>n</var> elements, a 
 * new element is inserted with probability proportional to 
 * {@link #insert_rate}+<var>n</var>&times;{@link #dup_rate}, 
 * and an existing one is deleted with probability proportional
 * to {@link #dup_rate}. The stationary distribution of the set size 
 * is then either Poisson (if {@link #dup_rate}=0) or negative binomial 
 * (if {@link #dup_rate}&gt; 0), with expectation 
 * {@link #insert_rate}/(1-{@link #dup_rate}). 
 * 
 * @author Mikl&oacute;s Cs&#369;r&ouml;s
 */
public final class SetTester 
{

    private static final int DEFAULT_SEED = new Random().nextInt();

    /**
     * Instantiation for testing a given implementation. 
     * 
     * @param test_set
     * @param insert_rate rate by which new elements are generated
     * @param dup_rate linear component of the insertion rate
     * @param random_seed seed for the pseudorandom number generator
     */
    public SetTester(Set<Object> test_set, double insert_rate, double dup_rate, int random_seed)
    {
        this.insert_rate = insert_rate;
        this.dup_rate = dup_rate;
        this.RND = new Random(random_seed);
        this.test_set = test_set;
    }

    private final double insert_rate;
    private final double dup_rate;
    private final Random RND;
    private final Set<Object> test_set;

    public double sizeMean()
    {
      return this.insert_rate / (1.0D - this.dup_rate);
    }

    public double sizeVariance()
    {
      return sizeMean() / (1.0D - this.dup_rate);
    }

    private final class OpSequenceGenerator implements Iterator<SetOperation>
    {
        private double current_time;
        private final PriorityQueue<Mortal> elements;
        private long current_ident;

        OpSequenceGenerator()
        {
            this.elements = new PriorityQueue<>();
            this.current_ident = 0L;
            this.current_time = 0.0;
        }

        /**
         * There is always a next one. 
         * 
         * @return true
         */
        @Override
        public boolean hasNext()
        {
          return true;
        }

        /**
         * Next operation: insertion or deletion.
         * 
         * @return 
         */
        public SetOperation next()
        {
            double tot_insert_rate = insert_rate + this.elements.size() * dup_rate;
            double next_bday = current_time + nextExponential(tot_insert_rate);
            SetOperation op;
            if (elements.isEmpty() || elements.peek().diesAfter(next_bday))
            {
                this.current_ident ++;
                op = insertOperation(new Long(this.current_ident));
                Mortal emt = new Mortal(this.current_ident, next_bday);
                this.elements.add(emt);
                this.current_time = emt.birth_date;
            } else 
            {
                Mortal emt = elements.poll();
                op = deleteOperation(new Long(emt.ident));
                this.current_time = emt.death_date;
            }
            return op;
        }

        /**
         * Next block of operations.
         * @param num_ops number of operations in the returned table
         * @return table with insertion and deletion operations.
        */
        SetOperation[] next(int num_ops)
        {
            SetOperation[] op_seq = new SetOperation[num_ops];
            for (int i = 0; i < num_ops; i++) op_seq[i] = next();
            return op_seq;
        }
    }

    /**
     * Operation with a fixed key, used by {@link OpSequenceGenerator}.
     */
    private abstract class SetOperation
    {
        private final Object key;

        SetOperation(Object key)
        {
            this.key = key;
        }

        /**
         * Deletion from the set.
         * @return whether the key was there.
         */
        final boolean delete() 
        {
           return test_set.remove(this.key);
        }

        /**
         * Insertion into the set.
         * 
         * @return whether a new key.
         */
        final boolean insert() 
        {
            return test_set.add(this.key);
        }

        /**
         * Search in the set.
         * 
         * @return whether the search is successful
         */
        final boolean search() 
        {
            return test_set.contains(this.key);
        }

        /**
         * Extending classes implement the actual operation performed here.
         * 
         * @return success of the operation
         */
        abstract boolean execute();
    }

    /**
     * Insertion of a key.
     * 
     * @param key
     * @return 
     */
    SetOperation insertOperation(Object key)
    {
        return new SetOperation(key)
        {
            @Override
            boolean execute() 
            {
                return insert();
            }
        };
    }
    
    /**
     * Deletion of a key.
     * 
     * @param key
     * @return 
     */
    SetOperation deleteOperation(Object key)
    {
        return new SetOperation(key)
        {
            @Override
            boolean execute()
            {
                return delete();
            }
        };
    }
    
    /**
     * Element with a life span (between insertion and removal). 
     */
    private class Mortal implements Comparable<Mortal>
    {
        private final double birth_date;
        private final double death_date;
        private final long ident;

        Mortal(long ident, double birth_date, double life_span)
        {
            this.birth_date = birth_date;
            this.death_date = (birth_date + life_span);
            this.ident = ident;
        }
        
        /**
         * Element with random life span (exponential distribution with mean 1).
         * @param ident identifier
         * @param bday  birth date
         */
        Mortal(long ident, double bday)
        {
            this(ident, bday, SetTester.this.nextExponential(1.0D));
        }

        /**
         * Comparison by death date. 
         * 
         * @param o
         * @return 
         */
        public int compareTo(Mortal o)
        {
            return Double.compare(this.death_date, o.death_date);
        }

        boolean diesAfter(double date)
        {
            return this.death_date > date;
        }
    }    

    /**
     * A random value by Exponential(lambda). Uses the classic transformation method. 
     *
     * @param lambda the rate parameter 
     * @return exponentially distributed random variable with expected value 1/<var>lambda</var>
     */    
    private double nextExponential(double lambda)
    {
        return -Math.log(this.RND.nextDouble()) / lambda;
    }

    /**
     * Used memory. Calls garbage collector a few times until memory usage settles. 
     * 
     * @return Java Virtual Machine used memory in bytes
     */
    private static long usedMemory()
    {
        Runtime R = Runtime.getRuntime();

        long current_use = Long.MAX_VALUE;
        int iter = 0;
        long prev_use;
        do
        {
            R.runFinalization();
            R.gc();
            prev_use = current_use;
            current_use = R.totalMemory() - R.freeMemory();
            iter++;
        } while ((current_use < prev_use) && (iter < 1000));
        return current_use;
    }
    
    /**
     * Snapshot of the execution after a series of operations, with information
     * about running time and memory usage.
     */
    public class ExecutionSnapshot 
    {
        public final int num_ops;
        public final long time_nanosec;
        public final long memory;
        public final int size;

        private ExecutionSnapshot(int num_ops, long delta, long mem, int size) 
        {
            this.num_ops=num_ops;
            this.time_nanosec = delta;
            this.memory = mem;
            this.size = size;
        }

        /**
         * Amortized time per operation
         * @return time in nanoneseconds
         */
        public final double amortizedTime() 
        {
            return time_nanosec / ((double) num_ops);
        }

        /**
         * Average memory usage per element.
         * 
         * @return bytes per element
         */
        public final double amortizedMemory() 
        {
            return memory / ((double) size);
        }
        
        /**
         * String for tabulated printing.
         * 
         * @return tab-separated values: number of operations, size, used memory (bytes), total execution time (nanoseconds), memory usage per element, amortized execution time per operation   
         */
        @Override
        public String toString()
        {
            StringBuilder sb = new StringBuilder();
            sb.append(this.num_ops);
            sb.append("\t").append(this.size);
            sb.append("\t").append(this.memory);
            sb.append("\t").append(this.time_nanosec);
            if (this.size != 0)
              sb.append("\t").append(amortizedMemory());
            if (this.num_ops != 0)
              sb.append("\t").append(amortizedTime());
            return sb.toString();
        }

        private String headerString()
        {
            return "num.ops\tsize\tmemory(bytes)\ttime(ns)\tmem/element\ttime/op";
        }
    }

    /**
     * Executes a number of random operations, and takes a snapshot. 
     * 
     * @param G
     * @param num_ops
     * @return 
     */
    private ExecutionSnapshot timeRandomSequence(OpSequenceGenerator G, int num_ops)
    {
        SetOperation[] ops = G.next(num_ops);
        long T0 = System.nanoTime();
        boolean b;
        for (SetOperation op : ops)
        {
          b = op.execute();
        }

        long T1 = System.nanoTime();
        ops = null;
        long mem = usedMemory();
        long elapsed_time = T1 - T0;
        return new ExecutionSnapshot(num_ops, elapsed_time, mem, test_set.size());
    }

    /**
     * Prints statistics about execution time and memory usage to the standard output. 
     * 
     * @param n_warmup number of warmup operations
     * @param n_ops number of timed operations
     */
    public void runTimings(int n_warmup, int n_ops)
    {
        OpSequenceGenerator G = new OpSequenceGenerator();
        ExecutionSnapshot init_snapshot = timeRandomSequence(G, 0);
        System.out.println("# Phase\t" + init_snapshot.headerString()+"\t// "+test_set.getClass().getCanonicalName());
        System.out.println("init\t" + init_snapshot);
        ExecutionSnapshot warmup_snapshot = timeRandomSequence(G, n_warmup);
        System.out.println("warmup\t" + warmup_snapshot);
        ExecutionSnapshot exec_snapshot = timeRandomSequence(G, n_ops);
        System.out.println("exec\t" + exec_snapshot); // useful for time
        G = new OpSequenceGenerator();
        ExecutionSnapshot final_snapshot = timeRandomSequence(G, 0);
        System.out.println("final\t" + final_snapshot); // gives proper memory usage (without the priority queue in the generator)
        this.test_set.clear();
    }

    public static void main(String[] args) throws Exception
    {
        java.util.Properties Props=System.getProperties();
        String system = Props.getProperty("os.name", "[unknown OS]")+" "+Props.getProperty("os.version","[Unknown version]")+" "+Props.getProperty("os.arch","[Unknown architecture]");
        String java = Props.getProperty("java.vm.name","[Unknown VM]")+" "+Props.getProperty("java.vm.version","[Unknown version]")+" ("+Props.getProperty("java.runtime.version","[Unknwon runtime]")+") "+Props.getProperty("java.vm.info","")+", "+Props.getProperty("java.vm.vendor","[Uknown vendor]");
        System.out.println("# System: "+system);
        System.out.println("# Java: "+java);

        int arg_idx = 0;

        int seed = DEFAULT_SEED;
        double dup_rate = 0.0D;
        int num_ops = 0;
        int num_warmup_ops = -1;
        double cv = 0.0;
        
        Set<Object> trythis = new HashSet<>(2048, 0.5f);

        while ((arg_idx < args.length) && (args[arg_idx].startsWith("-")))
        {
            String arg = args[(arg_idx++)];
            String val = arg_idx < args.length ? args[(arg_idx++)] : null;

            if ("-seed".equals(arg))
            {
                seed = Integer.parseInt(val);
            } else if ("-dispersion".equals(arg))
            {
                double vmr = Double.parseDouble(val);
                if (vmr < 1.0)
                    throw new IllegalArgumentException("Dispersion (variance-to-mean ratio) must be >= 1.0");
                dup_rate = 1.0 - 1.0 / vmr;
            } else if ("-cv".equals(arg)) 
            {
                cv = Double.parseDouble(val);
            }
            else if ("-n".equals(arg))
            {
                num_ops = Integer.parseInt(val);
            } else if ("-warmup".equals(arg))
            {
                num_warmup_ops = Integer.parseInt(val);
            } else if ("-type".equals(arg))
            {
            	switch(val) {
            	case "Lazy":
            		trythis = new LazySet(2048);
            		break;
            	case "Hasty":
            		trythis = new HastySet(2048);
            		break;
            	}
            }
        }
        if (arg_idx == args.length)
        {
            throw new IllegalArgumentException("Call with expected size.");
        }
        double exp_size = Double.parseDouble(args[(arg_idx++)]);
        
        if (cv !=0.0) // coefficient of variation = stdev / mean
        {
            double vmr = cv*cv*exp_size;
            dup_rate = 1.0 - 1.0 / vmr;
        }
        
        double ins_rate = (1.0-dup_rate)*exp_size;
        
        if (num_warmup_ops < 0)
          num_warmup_ops = (int)(2.0 * exp_size);
        if (num_ops == 0) 
            num_ops = 1000000;
        
        SetTester tester = new SetTester(trythis, ins_rate, dup_rate, seed);
//        System.out.println("#Test: mean size "+tester.sizeMean()+", variance "+tester.sizeVariance()+", sd "+Math.sqrt(tester.sizeVariance())+"; ins "+ins_rate+", dup "+dup_rate);

        tester.runTimings(num_warmup_ops, num_ops);
    }    
}