"""
Depth First Search
https://www.youtube.com/watch?v=9RHO6jU--GU

Breadth First Search
https://www.youtube.com/watch?v=86g8jAQug04

Requesting a family from the server:
family = Request_thread(f'{TOP_API_URL}/family/{id}')

Requesting an individual from the server:
person = Request_thread(f'{TOP_API_URL}/person/{id}')

10% Bonus to speed up part 3
"""
from common import *
import queue

# -----------------------------------------------------------------------------
def depth_fs_pedigree(family_id, tree: Tree):
    """Creates a pedigree using a depth search"""

    # Finishing case for recursive function
    if family_id == None or tree.does_family_exist(family_id):
        return
    
    family_request = Request_thread(f'{TOP_API_URL}/family/{family_id}')
    family_request.start()

    family_request.join()
    family = Family(family_request.response) # I removed the id param as it was unused
    tree.add_family(family)

    # Start the husband info request if his id exists and isn't already in the tree
    husband_request = None
    if family.husband != None and not tree.does_person_exist(family.husband):
        husband_request = Request_thread(f'{TOP_API_URL}/person/{family.husband}')
        husband_request.start()

    # Start the wife info request if her id exists and isn't already in the tree
    wife_request = None
    if family.wife != None and not tree.does_person_exist(family.wife):
        wife_request = Request_thread(f'{TOP_API_URL}/person/{family.wife}')
        wife_request.start()
        
    # Add any children requests if they aren't already in the tree
    children_requests: list[Request_thread] = []
    for child in family.children:
        if not tree.does_person_exist(child):
            children_requests.append(Request_thread(f'{TOP_API_URL}/person/{child}'))

    # Start the children requests (if any)
    for r in children_requests:
        r.start()

    # Threads list contains the threads that use the function recursively
    threads: list[threading.Thread] = []

    # Add the husband if we we sent a request
    if husband_request != None:
        husband_request.join()
        if husband_request.response: # Add only if there was an actual response
            husband = Person(husband_request.response)
            tree.add_person(husband)
            if husband.parents != None: # Add a thread if we have more families to add
                threads.append(threading.Thread(target=depth_fs_pedigree, args=(husband.parents, tree)))

    # Add the wife if we sent a request
    if wife_request != None:
        wife_request.join()
        if wife_request.response: # Add only if there was an actual response
            wife = Person(wife_request.response)
            tree.add_person(wife)
            if wife.parents != None: # Add a thread if we have more families to add
                threads.append(threading.Thread(target=depth_fs_pedigree, args=(wife.parents, tree)))
    
    # Start the recursive threads
    for t in threads:
        t.start()

    # Add any children that we sent requests for
    for request in children_requests:
        request.join()
        if request.response: # Add only if there was an actual response
            child = Person(request.response)
            tree.add_person(child)

    # Wait for all recursive threads to finish
    for t in threads:
        t.join()
        

# -----------------------------------------------------------------------------
def add_family(family_id, tree: Tree, queue: queue.Queue):
    """Adds a family to the pedigree"""
    # We are done with the search
    if family_id == None:
        return
    
    # Get the family
    family_request = Request_thread(f'{TOP_API_URL}/family/{family_id}')
    family_request.start()

    family_request.join()
    family = Family(family_request.response) # I removed the id param as it was unused
    tree.add_family(family)

    # Start the husband info request if his id exists and isn't already in the tree
    husband_request = None
    if family.husband != None and not tree.does_person_exist(family.husband):
        husband_request = Request_thread(f'{TOP_API_URL}/person/{family.husband}')
        husband_request.start()

    # Start the wife info request if her id exists and isn't already in the tree
    wife_request = None
    if family.wife != None and not tree.does_person_exist(family.wife):
        wife_request = Request_thread(f'{TOP_API_URL}/person/{family.wife}')
        wife_request.start()
        
    # Add any children requests if they aren't already in the tree
    children_requests: list[Request_thread] = []
    for child in family.children:
        if not tree.does_person_exist(child):
            children_requests.append(Request_thread(f'{TOP_API_URL}/person/{child}'))

    # Start the children requests (if any)
    for r in children_requests:
        r.start()

    # This determines if we have more families to get
    any_more_families = False

    # Add the husband if we we sent a request
    if husband_request != None:
        husband_request.join()
        if husband_request: # Add only if there was an actual response
            husband = Person(husband_request.response)
            tree.add_person(husband)
            if husband.parents != None: # Add a thread if we have more families to add
                queue.put(husband.parents)
                any_more_families = True

    # Add the wife if we sent a request
    if wife_request != None:
        wife_request.join()
        if wife_request.response: # Add only if there was an actual response
            wife = Person(wife_request.response)
            tree.add_person(wife)
            if wife.parents != None: # Add a thread if we have more families to add
                queue.put(wife.parents)
                any_more_families = True

    # Add any children that we sent requests for
    for request in children_requests:
        request.join()
        if request.response: # Add only if there was an actual response
            child = Person(request.response)
            tree.add_person(child)

    # Let everyone know we are done
    if not any_more_families:
        queue.put("DONE")
        

def breadth_fs_pedigree(start_id, tree: Tree):
    """Creates a family history pedigree using breadth search"""
    q = queue.Queue()
    q.put(start_id)
    threads: list[threading.Thread] = []

    while True:
        family_id = q.get()

        if family_id == "DONE":
            break
        
        if family_id != None:
            t = threading.Thread(target=add_family, args=(family_id, tree, q))
            t.start()
            threads.append(t)

    for t in threads:
        t.join()


# -----------------------------------------------------------------------------
class Request_thread_limit5(threading.Thread):

    def __init__(self, url, sem:threading.Semaphore):
        # Call the Thread class's init function
        threading.Thread.__init__(self)
        self.url = url
        self.response = {}
        self.sem = sem

    def run(self):
        self.sem.acquire()
        response = requests.get(self.url)
        # Check the status code to see if the request succeeded.
        if response.status_code == 200:
            self.response = response.json()
        else:
            print('RESPONSE = ', response.status_code)
        self.sem.release()
        

def add_family_limit5(family_id, tree: Tree, queue: queue.Queue, thread_sem: threading.Semaphore):
    """Adds a family to the pedigree with a limit of 5 threads"""
    # We are done with the search
    if family_id == None:
        return
    
    # Get the family
    family_request = Request_thread_limit5(f'{TOP_API_URL}/family/{family_id}', thread_sem)
    family_request.start()

    family_request.join()
    family = Family(family_request.response) # I removed the id param as it was unused
    tree.add_family(family)

    # Start the husband info request if his id exists and isn't already in the tree
    husband_request = None
    if family.husband != None and not tree.does_person_exist(family.husband):
        husband_request = Request_thread_limit5(f'{TOP_API_URL}/person/{family.husband}', thread_sem)
        husband_request.start()

    # Start the wife info request if her id exists and isn't already in the tree
    wife_request = None
    if family.wife != None and not tree.does_person_exist(family.wife):
        wife_request = Request_thread_limit5(f'{TOP_API_URL}/person/{family.wife}', thread_sem)
        wife_request.start()
        
    # Add any children requests if they aren't already in the tree
    children_requests: list[Request_thread_limit5] = []
    for child in family.children:
        if not tree.does_person_exist(child):
            t = Request_thread_limit5(f'{TOP_API_URL}/person/{child}', thread_sem)
            t.start()
            children_requests.append(t)

    # This determines if we have more families to get
    any_more_families = False

    # Add the husband if we we sent a request
    if husband_request != None:
        husband_request.join()
        if husband_request: # Add only if there was an actual response
            husband = Person(husband_request.response)
            tree.add_person(husband)
            if husband.parents != None: # Add a thread if we have more families to add
                queue.put(husband.parents)
                any_more_families = True

    # Add the wife if we sent a request
    if wife_request != None:
        wife_request.join()
        if wife_request.response: # Add only if there was an actual response
            wife = Person(wife_request.response)
            tree.add_person(wife)
            if wife.parents != None: # Add a thread if we have more families to add
                queue.put(wife.parents)
                any_more_families = True

    # Add any children that we sent requests for
    for request in children_requests:
        request.join()
        if request.response: # Add only if there was an actual response
            child = Person(request.response)
            tree.add_person(child)

    # Let everyone know we are done
    if not any_more_families:
        queue.put("DONE")

def breadth_fs_pedigree_limit5(start_id, tree):
    thread_sem = threading.Semaphore(5)
    q = queue.Queue()
    q.put(start_id)
    threads: list[threading.Thread] = []

    while True:
        family_id = q.get()

        if family_id == "DONE":
            break
        elif family_id != None:
            t = threading.Thread(target=add_family_limit5, args=(family_id, tree, q, thread_sem))
            t.start()
            threads.append(t)

    for t in threads:
        t.join()

