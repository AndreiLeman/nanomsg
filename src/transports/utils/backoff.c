/*
    Copyright (c) 2013 250bpm s.r.o.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "backoff.h"

void nn_backoff_init (struct nn_backoff *self, int minivl, int maxivl,
    struct nn_fsm *owner)
{
    nn_timer_init (&self->timer, owner);
    self->minivl = minivl;
    self->maxivl = maxivl;
    self->n = 1;
}

void nn_backoff_term (struct nn_backoff *self)
{
    nn_timer_term (&self->timer);
}

int nn_backoff_isidle (struct nn_backoff *self)
{
    return nn_timer_isidle (&self->timer);
}

void nn_backoff_start (struct nn_backoff *self)
{
     int timeout;

     /*  Start the timer for the actual n value. If the interval haven't yet
         exceeded the maximum, double the next timeout value. */
     timeout = (self->n - 1) * self->minivl;
     if (timeout > self->maxivl)
         timeout = self->maxivl;
     else
         self->n *= 2;
     nn_timer_start (&self->timer, timeout);
}

void nn_backoff_stop (struct nn_backoff *self)
{
    nn_timer_stop (&self->timer);
}

void nn_backoff_reset (struct nn_backoff *self)
{
    self->n = 1;
}

