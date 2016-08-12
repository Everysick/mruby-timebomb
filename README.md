# mruby-timebomb

## example

```ruby
> tb1 = Timebomb.new(100) { |val| puts "foo"; val }
=> #<Timebomb:0x7fbc320082a0>
> tb1.call(100)
foo
=> 100
> tb2 = Timebomb.new(1) { |val| puts "foo"; val }
=> #<Timebomb:0x7fbc32006920>
> tb2.call(100)
foo
(mirb):4: Over the time limit (RuntimeError)
```
