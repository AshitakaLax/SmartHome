from django.conf.urls import patterns, include, url

urlpatterns = patterns('website.views',
    url(r"^$", "index"),
    url(r"^view_all/$", "view_all"),
    url(r"^change/$", "change")
)
