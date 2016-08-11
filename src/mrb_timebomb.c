#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <time.h>

static void
mrb_timebomb_init(mrb_state *mrb, mrb_value self)
{
	mrb_value block;
	mrb_int time_limit;

	mrb_get_args(mrb, "i&", &time_limit, &block);

	if (time_limit <= 0) {
		mrb_raise(mrb, E_TYPE_ERROR, "Please enter a natural number");
	}

	if (mrb_nil_p(block)) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
	}

	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "time_limit"), mrb_fixnum_value(time_limit));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "block"), block);
}

static mrb_value
mrb_timebomb_call(mrb_state *mrb, mrb_value self)
{
	struct RClass *c;
	mrb_value *argv;
	mrb_int argc;

	clock_t start, end;
	mrb_value block, val;
	mrb_value time_limit;
	mrb_int num;

	block = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "block"));
	time_limit = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "time_limit"));
	num = mrb_fixnum(time_limit);

	c = mrb_class_ptr(mrb_singleton_class(mrb, self));
	mrb_get_args(mrb, "*", &argv, &argc);

	start = clock();
	val = mrb_yield_with_class(mrb, block, argc, argv, self, c);
	end = clock();

	if ((int)(end - start) <= num) {
		return val;
	} else {
		mrb_raise(mrb, E_RUNTIME_ERROR, "Over the time limit");
		return mrb_nil_value();
	}
}

void
mrb_mruby_timebomb_gem_init(mrb_state *mrb)
{
	struct RClass *timebomb;

	timebomb = mrb_define_class(mrb, "Timebomb", mrb->object_class);
	MRB_SET_INSTANCE_TT(timebomb, MRB_TT_DATA);

	mrb_define_method(mrb, timebomb, "initialize", mrb_timebomb_init, MRB_ARGS_REQ(1) | MRB_ARGS_BLOCK());
	mrb_define_method(mrb, timebomb, "call", mrb_timebomb_call, MRB_ARGS_ANY());

	return;
}

void
mrb_mruby_timebomb_gem_final(mrb_state *mrb)
{
	return;
}
