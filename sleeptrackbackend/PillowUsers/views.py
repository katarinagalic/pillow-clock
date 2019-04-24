from django.shortcuts import render, redirect, get_object_or_404
from .forms import UserForm
from .models import User


def create_user(request):

    if request.method == "POST":
        form = UserForm(request.POST)
        if form.is_valid():
            user = form.save(commit=False)
            user.save()
            return redirect('edit_user', current=user.username)
    else:
        form = UserForm()
    return render(request, 'new_user.html', {'form': form})


def edit_user(request, current):
    user = get_object_or_404(User, username=current)
    if request.method == "POST":
        form = UserForm(request.POST, instance=user)
        if form.is_valid():
            user = form.save(commit=False)
            user.save()
            return redirect('edit_user', current=current)
    else:
        form = UserForm(instance=user)
    return render(request, 'new_user.html', {"form": form})


def user_list(request):
    users = User.objects.all()

    return render(request, 'users.html', {"users": users})
