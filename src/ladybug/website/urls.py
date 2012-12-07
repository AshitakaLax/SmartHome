from django.conf.urls import patterns, include, url

urlpatterns = patterns('website.views',
    url(r"^$", "view"),
    url(r"^change/$", "change"),
    url(r"^view_all/$", "view_all"),
    #url(r"^view_all/change/$", "change")
)
