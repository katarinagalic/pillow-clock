import PIL, PIL.Image
import io
import json

from django.contrib import messages
from django.core import serializers
from django.http import JsonResponse, HttpResponse, HttpResponseBadRequest, HttpResponseForbidden
from django.shortcuts import render, redirect, get_object_or_404
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods

from .forms import UserAlarmForm, NightForm, UserForm
from .models import User_Alarm
from matplotlib import pylab
import matplotlib.pyplot as pp
import matplotlib as plt
from pylab import *

def create_user(request):
    try:
        if request.method == "POST":
            form = UserForm(request.POST)
            # if form.is_valid():
            print(request.POST)
            print(form)
            user = form.save(commit=False)
            user.save()
            messages.info(request, 'Thank you for entering a username! You can see your data in the Users tab.')
            # return redirect('edit_user', current=user.username)
        else:
            form = UserForm()
        return render(request, 'new_user.html', {'form': form, 'user': None})
    except ValueError:
        form = UserForm()
        messages.info(request, 'You have already submitted this username.')
        return render(request, 'new_user.html', {'form': form, 'user': None})


@csrf_exempt
@require_http_methods(["POST", "GET"])
def edit_user(request, current):
    user = get_object_or_404(User_Alarm, user_id=current)
    if request.method == "POST":
        form = UserAlarmForm(request.POST, instance=user)
        if form.is_valid():
            user = form.save(commit=False)
            user.save()
            return redirect('edit_user', current=current)
        else:
            return HttpResponseBadRequest(str(form.errors))
    else:
        if "application/json" in request.META.get('HTTP_ACCEPT'):
            return HttpResponseForbidden()
        else:
            form = UserAlarmForm(instance=user)
            return render(request, 'new_user.html', {"form": form, 'user': user})


@require_http_methods(["GET"])
def sleeptrack(request, current):
    user = get_object_or_404(User_Alarm, user_id=current)
    if request.method == 'GET':
        result=user.night_set.all()
        if ("application/json" in request.META.get("HTTP_ACCEPT")):
            result=json.loads(serializers.serialize('json', result))
            return JsonResponse({"data":result})
        elif True or ("image/png" in request.META.get("HTTP_ACCEPT")):
            x = []
            y = []

            for night in result:
                x.append(night.day_id)
                y.append(night.conversion())

            pp.bar(x,y, width=0.5)
            plt.xlabel('Day')
            plt.ylabel('Time')
            plt.title('Sleepdata')
            pp.xticks(range(len(x)+1))



            buffer = io.BytesIO()
            canvas = pylab.get_current_fig_manager().canvas
            canvas.draw()
            pilImage = PIL.Image.frombytes("RGB", canvas.get_width_height(), canvas.tostring_rgb())
            pilImage.save(buffer, "PNG")
            pylab.close()
            return HttpResponse(buffer.getvalue(), content_type='image/png')
        else:
            return HttpResponse(list(result))

@csrf_exempt
@require_http_methods(["GET", "POST"])
def nights(request):
    if request.method == "POST":
        form = NightForm(request.POST)
        print(form)
        if form.is_valid():
            form.save()
            return redirect('/users/')
        else:
            return HttpResponseBadRequest(str(form.errors))
        pass
    else:
        form = NightForm()
        return render(request, 'sleeptrack.html', {"form": form})



def user_list(request):
    users = User_Alarm.objects.all()
    print(users)
    return render(request, 'users.html', {"users": users})

@csrf_exempt
def about_page(request):
    return render(request, 'about.html')

@csrf_exempt
def hardware_page(request):
    return render(request, 'system.html')