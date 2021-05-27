# ThreadPool
C++ simpale ThreadPool

* support sync call in the same thread.
the calling thread need to waits for the worker thread.

      worker.sync_call([]{
    	int ans = 0;
    	for(int i = 1; i <= 100; i++)
    		ans += i;
    	return ans;
      });

* support async call in the same thread.
the calling thread does not need to wait for the worker thread.

	  worker.async_call([]{
	    do_something();
	  });
