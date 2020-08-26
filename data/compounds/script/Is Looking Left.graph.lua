return {
  links = {
    {
      1,
      1,
      5,
      2,
    },
    {
      3,
      1,
      5,
      1,
    },
    {
      5,
      1,
      7,
      1,
    },
    {
      6,
      1,
      3,
      1,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      loadArguments = {
        'pi',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        '(Heading - pi * 0.25) % (2 * pi)',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        {
          3,
          'Heading',
        },
      },
      name = 'script/scriptinput',
    },
    {
      loadArguments = {
        {
          1,
          'Result',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}