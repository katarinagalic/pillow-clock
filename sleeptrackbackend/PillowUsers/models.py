from django.db import models


class User(models.Model):

    username = models.CharField(max_length=100, unique=True)
    alarm_time = models.TimeField()

    def __str__(self):
        return self.username + str(self.alarm_time)

# Create your models here.