from django.core.urlresolvers import reverse, NoReverseMatch
from django.template import Template, RequestContext

def users(request):
    if request.user.is_authenticated():
        user_is_staff = request.user.is_staff
        return {"username": request.user.username,
                "user_is_staff": user_is_staff}
    else:
        return {"username": None, "user_is_staff": False}


def django_template_constructs(request):
    context_vars_dict = {}
    
    def add_function_to_context(func):
        context_vars_dict[func.__name__] = func
    
    @add_function_to_context
    def url(lookup_string, *args, **kwargs):
        #try:
            return reverse(lookup_string, args=args, kwargs=kwargs)
        # except NoReverseMatch as e:
        #     import sys
        #     print >> sys.stderr, e
        #     return "/unfinished/" + lookup_string + "/"
            
    @add_function_to_context
    def csrf_protect():
        return Template("{% csrf_token %}").render(RequestContext(request))
        
    return context_vars_dict
