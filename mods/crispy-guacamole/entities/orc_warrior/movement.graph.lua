script {
  nodes = {
    {
      name = 'components/movementcomponent',
    },
    {
      loadArguments = {
        2.5,
      },
      name = 'number',
    },
    {
      loadArguments = {
        10,
      },
      name = 'number',
    },
    {
      loadArguments = {
        2,
      },
      name = 'number',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
  },
  links = {
    {
      1,
      1,
      5,
      1,
    },
    {
      2,
      1,
      1,
      2,
    },
    {
      3,
      1,
      1,
      3,
    },
    {
      4,
      1,
      1,
      1,
    },
    {
      6,
      1,
      1,
      4,
    },
  },
}