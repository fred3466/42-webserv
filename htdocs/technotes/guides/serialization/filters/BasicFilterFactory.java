/*
 * Copyright (c) 2021, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import sun.misc.ObjectInputFilter;
import sun.misc.ObjectInputFilter.FilterInfo;
import java.util.function.BinaryOperator;

/**
 * A simple example of a filter factory. It prints its
 * sun.misc.ObjectInputFilter parameters every time its apply method is invoked,
 * then returns an ObjectInputFilter that consists of the apply method's
 * parameters merged into one filter.  
 */
    
public class BasicFilterFactory implements BinaryOperator<ObjectInputFilter> {

    public ObjectInputFilter apply(ObjectInputFilter curr,
        ObjectInputFilter next) {
        System.out.println("Current filter: " + curr);
        System.out.println("Requested filter: " + next);
        if (curr == null)
            return next;
        if (next == null)
            return curr;
        return (FilterInfo info) -> {
            ObjectInputFilter.Status currStatus = curr.checkInput(info);
            if (ObjectInputFilter.Status.REJECTED.equals(currStatus)) {
                return ObjectInputFilter.Status.REJECTED;
            }
            ObjectInputFilter.Status nextStatus = next.checkInput(info);
            if (ObjectInputFilter.Status.REJECTED.equals(nextStatus)) {
                return ObjectInputFilter.Status.REJECTED;
            }
            if (ObjectInputFilter.Status.ALLOWED.equals(currStatus)
                || ObjectInputFilter.Status.ALLOWED.equals(nextStatus)) {
                return ObjectInputFilter.Status.ALLOWED;
            }
            return ObjectInputFilter.Status.UNDECIDED;
        };
    }
}