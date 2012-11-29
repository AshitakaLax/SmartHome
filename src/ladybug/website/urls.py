from django.conf.urls import patterns, include, url

urlpatterns = patterns('website.views',
    url(r"^$", "index"),
    url(r"^view/$", "view"),
    url(r"^change/(\d+)/$", "change")
)
